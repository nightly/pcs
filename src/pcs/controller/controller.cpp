#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/ranges.h>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {

	using TopologyTransition = std::pair<size_t, std::string>;

	Controller::Controller(const System* machine, ITopology* topology, const Recipe* recipe)
		: machine_(machine), recipe_(recipe), topology_(topology), num_of_resources_(machine_->NumOfResources()) {}

	std::optional<const LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> Controller::Generate() {
		recipe_state_ = recipe_->lts().initial_state();
		topology_state_ = topology_->initial_state();
		controller_.set_initial_state(topology_state_, true);
		parts_.resize(machine_->NumOfResources(), std::vector<std::string>());

		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller initial state {}", fmt::join(controller_.initial_state(), ",")));
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Recipe initial state: {}", recipe_state_));

		bool generated = ProcessRecipe(recipe_state_);
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller generation completed: realisability = {}", generated));
		if (!generated) {
			return {};
		}
		return &controller_;
	}

	bool Controller::ProcessRecipe(const std::string& state) {
		if (recipe_->lts()[state].transitions_.empty()) {
			return true; // No further recipe states to process
		}

		// Since recipes have guard conditions, we account for all possibilities
		for (const auto& pair : recipe_->lts()[state].transitions_) {
			const CompositeOperation& co = recipe_->lts()[state].transitions_[0].first; // assume 1st branch as guard passing pathway
			PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold, "Processing recipe transition to: {}",
				recipe_->lts()[state].transitions_[0].second));

			bool handle = HandleComposite(co);
			if (handle == false) {
				return false;
			}

			const std::string& next_recipe_state = recipe_->lts()[state].transitions_[0].second; // assume 1st branch as guard passing pathway
			ProcessRecipe(next_recipe_state);
		}

	}


	/**
	 * @brief Handles a Composite Operation type, the Transition type present within Recipe States/LTS 
	 */
	bool Controller::HandleComposite(const CompositeOperation& co) {
		for (const auto& tuple : co.sequential) {
			seq_tuple_ = &tuple;
			const auto& [op, input, output] = *seq_tuple_;
			PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "Handling operation: {} with input parts {} and output parts {}",
				op.name(), fmt::join(input, ","), fmt::join(output, ",")));

			std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> plan_transitions;
			std::vector<std::vector<std::string>> plan_parts = parts_;
			bool operationSuccess = HandleSequentialOperation(topology_state_, plan_transitions, plan_parts);
			if (!operationSuccess) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * @brief Handles a single sequential operation. Returns true if it can be realised, false otherwise. 
	 */
	bool Controller::HandleSequentialOperation(const std::vector<std::string>& topology_state, std::vector<std::pair<std::vector<std::string>,
		                                       std::vector<std::string>>> plan_transitions, std::vector<std::vector<std::string>> plan_parts) {
		const auto& [op, input, output] = *seq_tuple_;

		// map - TransferOperation key, tuple<end_state, transition, inverse transition>
		std::map<TransferOperation, std::tuple<const std::vector<std::string>*, const TopologyTransition*, 
			const TopologyTransition*>> transfers;

		for (const auto& transition : topology_->at(topology_state).transitions_) {
			if (op.name() == transition.first.second) { // Found operation, as long as parts match, the plan for the current seq op passes
				parts_ = plan_parts; // We use the parts that are from our current plan
				bool transfer = TransferParts(transition.first.first, parts_);
				if (!transfer) {
					return false;
				}
				std::vector<std::string> vec(num_of_resources_, "-");
				vec[transition.first.first] = transition.first.second;

				plan_transitions.emplace_back(std::make_pair(vec, transition.second));
				for (const auto& transition : plan_transitions) { // We can now apply our correct transitions
					ApplyTransition(transition);
				}
				return true;
			}
			else {
				// Not the desired operation — is it a transfer operation?
				std::optional<TransferOperation> opt = StringToTransfer(transition.first.second); 
				if (opt.has_value()) {
					if (opt->IsOut()) { // Out case
						std::get<0>(transfers[*opt]) = &(transition.second);
						std::get<1>(transfers[*opt]) = &(transition.first);
					}
					else { // In case
						TransferOperation inverse = opt->Inverse(); // The associated map key is the inverse
						std::get<2>(transfers[inverse]) = &(transition.first);
					}
				}
			}
		}

		bool found = false;
	

		// Move between resources - try build a plan for each transfer
		for (const auto& [k, v] : transfers) {
			// map type =  [ TransferOperation key, tuple<end_state, transition, inverse transition> ]

			if (k.Inverse().name() != std::get<2>(v)->second) {
				PCS_INFO("Not equal {} and {}", k.Inverse().name(), std::get<2>(v)->second);
				// continue;
			}

			const std::vector<std::string>& state_vec = *std::get<0>(v);
			std::vector<std::string> label_vec(num_of_resources_, "-");
			label_vec[std::get<1>(v)->first] = k.name();
			label_vec[std::get<2>(v)->first] = std::get<2>(v)->second;

			plan_transitions.emplace_back(std::make_pair(label_vec, state_vec));

			found = HandleSequentialOperation(state_vec, plan_transitions, plan_parts); // recurse and try this new pathway
			if (found) {
				return true;
				break;
			}
		}
		return false;
	}


	/*
	 * @brief ApplyTransition will add a transition to the controller and update the current topology state
	 * pair.first = label, pair.second = end_state
	 */
	void Controller::ApplyTransition(const std::pair<std::vector<std::string>, std::vector<std::string>>& transition) {
		controller_.AddTransition(topology_state_, VectorToString(transition.first), transition.second);
		topology_state_ = transition.second;
		PCS_INFO(fmt::format(fmt::fg(fmt::color::royal_blue) | fmt::emphasis::bold,
			"Adding controller transition ({}) to {}", fmt::join(transition.first, ","), fmt::join(transition.second, ",")));
	}

	bool Controller::TransferParts(size_t resource, std::vector<std::vector<std::string>>& parts) {
		auto& [op, input, output] = *seq_tuple_; // Our current SequentialOperation with op, input, output -- probably shouldn't be a class variable
		std::vector<size_t> removals;

		if (!input.empty() && !parts[resource].empty()) {
			for (size_t i = 0; i < parts[resource].size(); ++i) {
				// Does our current part in the resource coincide with any part in our input we're looking for?
				if (std::find(input.begin(), input.end(), parts[resource][i]) != input.end()) {
					removals.emplace_back(i);
				}
			}
		}

		if (removals.size() != input.size()) { // If the size of input parts doesn't match removals size, we haven't found everything, ret false
			// PCS_WARN("Recipe is not reliasable, input parts cannot be found at resource index {}. Missing parts = {}", resource, fmt::join(input, ","));
		}

		for (const auto& i : removals) { // Now we can remove everything the recipe has consumed
			// parts_[resource].erase(parts_[resource].begin() + i);
		}


		for (const auto& s : output) {
			// parts_[resource].emplace_back(s);
		}

		return true;
	}

} // namespace end

 /*
	 @Cleanup: pair inception should probably be eliminated with dedicated Transition type -- this is present in lts/transition.h but we don't use it for now since
	 it is simply a readability matter for now.

	 @Todo: guard operation in each recipe state being handled

 */