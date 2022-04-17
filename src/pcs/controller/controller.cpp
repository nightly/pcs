#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include <spdlog/fmt/ranges.h>
#include <spdlog/fmt/bundled/color.h>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "pcs/operation/label.h"

namespace pcs {

	Controller::Controller(const Machine* machine, const Recipe* recipe) 
		: machine_(machine), recipe_(recipe), topology_(&machine->topology()), num_of_resources_(machine_->NumOfResources()) {}
	
	std::optional<const LTS<std::string, std::string>*> Controller::Generate() {
		parts_.resize(machine_->NumOfResources());
		recipe_state_ = recipe_->lts().initial_state();
		topology_state_ = topology_->initial_state();
		controller_.set_initial_state(topology_state_, true);

		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller initial state {}", controller_.initial_state()));
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

		const CompositeOperation& co = recipe_->lts()[state].transitions_[0].first; // assume 1st branch as guard passing pathway
		PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold, "Processing recipe transition to: {} (guard check passed)",
			recipe_->lts()[state].transitions_[0].second));

		bool handle = HandleComposite(co);
		if (handle == false) {
			return false;
		}

		const std::string& next_recipe_state = recipe_->lts()[state].transitions_[0].second; // assume 1st branch as guard passing pathway
		ProcessRecipe(next_recipe_state);
	}


	bool Controller::HandleComposite(const CompositeOperation& co) {
		for (const auto& tuple : co.sequential) {
			seq_tuple_ = &tuple;
			const auto& [op, input, output] = *seq_tuple_;
			PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "Handling operation: {} with input parts {} and output parts {}",
				op.name(), input, output));

			std::vector<std::pair<std::vector<std::string>, std::string>> plan_transitions;
			std::vector<std::vector<std::string>> plan_parts;
			bool operationSuccess = HandleSequentialOperation(topology_state_, plan_transitions, plan_parts);
			if (!operationSuccess) {
				return false;
			}
		}
		return true;

	}

	bool Controller::HandleSequentialOperation(std::string topology_state,
	std::vector<std::pair<std::vector<std::string>, std::string>> plan_transitions, std::vector<std::vector<std::string>> plan_parts) {
		const auto& [op, input, output] = *seq_tuple_;

		std::map<TransferOperation, std::tuple<std::string, TransferOperation, std::string>> transfers; // out, out_state, in, in_state
		for (const auto& transition : (*topology_)[topology_state].transitions_) {
			if (op.name() == transition.first) { // Found operation, as long as parts match, the plan for the current seq op passes
				size_t current_resource = CurrentResource(topology_state_, transition.second);

				bool transfer = TransferParts(transition, current_resource);
				if (!transfer) {
					return false;
				}
				std::vector<std::string> vec(num_of_resources_, "-");
				vec[current_resource] = transition.first;

				plan_transitions.emplace_back(std::make_pair(vec, transition.second));
				for (const auto& transition : plan_transitions) {
					ApplyTransition(transition);
				}
				return true;
			} else {
				// @Cleanup: types for LTS require a conversion
				std::optional<TransferOperation> opt = StringToTransfer(transition.first);
				if (opt.has_value()) {
					if (opt->Out()) {
						std::get<0>(transfers[*opt]) = transition.second;
					} else {
						TransferOperation inverse = opt->Inverse();
						std::get<1>(transfers[*opt]) = *opt;
						std::get<2>(transfers[*opt]) = transition.second;
					}
				}
			}
		}

		// Move between resources - try build a plan for each transfer
		for (auto& [k, v] : transfers) {
			std::string state;
			std::vector<std::string> state_vec = StringToVector(topology_state);

			std::vector<std::string> label_vec(num_of_resources_, "-");
			size_t transfer_resource_1, transfer_resource_2; 
			transfer_resource_1 = CurrentResource(topology_state, std::get<0>(v));
			state_vec[transfer_resource_1] = StringToVector(std::get<0>(v))[transfer_resource_1];
			label_vec[transfer_resource_1] = k.name();

			transfer_resource_2 = CurrentResource(topology_state, std::get<2>(v));
			state_vec[transfer_resource_2] = StringToVector(std::get<2>(v))[transfer_resource_2];
			label_vec[transfer_resource_2] = std::get<1>(v).name();

			state = VectorToString(state_vec);
			plan_transitions.emplace_back(std::make_pair(label_vec, state));
			PCS_INFO("{} {}", transfer_resource_1, transfer_resource_2);


			// 
			for (const auto& transition : plan_transitions) {
				ApplyTransition(transition);
			}
			//

			bool found = false;
			// bool found = HandleSequentialOperation(state, plan_transitions, plan_parts); // recurse
			if (found) {
				return true;
			}
		}

		return false; 
	}


	/*
	 * @brief ApplyTransition will add a transition to the controller and update the current topology state
	 */
	void Controller::ApplyTransition(const std::pair<std::vector<std::string>, std::string>& transition) {
		controller_.AddTransition(topology_state_, VectorToString(transition.first), transition.second);
		topology_state_ = transition.second;
		PCS_INFO(fmt::format(fmt::fg(fmt::color::royal_blue) | fmt::emphasis::bold,
			"Adding controller transition ({}) to {}.", VectorToString(transition.first), transition.second));
	}


	// Identifies current resource being reviewed based on old topology string state and new string state
	size_t Controller::CurrentResource(const std::string& old_state, const std::string& new_state) {
		std::vector<std::string> old_ve = StringToVector(old_state);
		std::vector<std::string> new_ve = StringToVector(new_state);
		for (size_t i = 0; i < new_ve.size(); ++i) {
			if (old_ve[i] != new_ve[i])
				return i;
		}
	}

	bool Controller::TransferParts(const std::pair<std::string, std::string>& transition, size_t current_resource) {
		/*
		if (!input.empty()) { // Check the input parts are present at the resource
			for (const auto& required_part : input) {
				if (std::find(parts_[current_resource].begin(), parts_[current_resource].end(), required_part) == parts_[current_resource].end()) {
					// ... Remove part ... from parts[n][] as the operation has consumed it
				} else {
					PCS_WARN("Recipe is unreliasable due to missing part {}", required_part);
				}
			}
		}
		for (const auto& s : output) {
			parts_[current_resource].push_back(s);
		} 
		*/
		return true;
	}

} // namespace end

 /*
	 @Cleanup: better types of LTS', e.g. topology KeyT is actually std::vector<std::string> not just a string - etc docs/overview.md

	 @Todo: use LTS template parameters, i.e. vec<std::string> instead of std::string only
	 @Todo: separate logic for handling each operation
	 @Todo: guard operations, handle failure case
 */
