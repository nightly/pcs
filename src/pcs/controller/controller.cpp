#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/ranges.h>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {

	using TopologyTransition = std::pair<size_t, std::string>;
	using TopologyState = std::vector<std::string>;

	Controller::Controller(const System* machine, ITopology* topology, const Recipe* recipe)
		: machine_(machine), recipe_(recipe), topology_(topology), num_of_resources_(machine_->NumOfResources()), 
		  parts_(machine_->NumOfResources()) {}

	std::optional<const LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> Controller::Generate() {
		recipe_state_ = recipe_->lts().initial_state();
		controller_.set_initial_state(topology_->initial_state(), true);

		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller initial state {}", fmt::join(controller_.initial_state(), ",")));
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Recipe initial state: {}", recipe_state_));

		bool generated = ProcessRecipe(recipe_state_, &controller_.initial_state());
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller generation completed: realisability = {}", generated));

		/*  @Hack: Visualises controller no matter what (wherever it reached) for testing purposes.
		 *	However, doesn't affect above realisability output though
		 */
		return &controller_;

		if (!generated) {
			return {};
		}
		return &controller_;
	}

	/**
	 * @brief Recursively process all recipe states, accounting for transitions due to guards.  
	 */
	bool Controller::ProcessRecipe(const std::string& recipe_state, const std::vector<std::string>* topology_state) {
		for (const auto& pair : recipe_->lts()[recipe_state].transitions_) {
			const CompositeOperation& co = pair.label();
			PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold, "Processing recipe transition to: {}",
				recipe_->lts()[recipe_state].transitions_[0].to()));

			auto c_op = HandleComposite(co, *topology_state);
			if (!c_op.has_value()) {
				return {};
			}
			ProcessRecipe(pair.to(), c_op.value());
		}
	}


	/**
	 * @brief Handles a Composite Operation type, the Transition type present within Recipe States/LTS 
	 */
	std::optional<const TopologyState*> Controller::HandleComposite(const CompositeOperation& co, const TopologyState& topology_state) {
		const std::vector<std::string>* res_state = &topology_state;
		for (const auto& tuple : co.sequential) {
			seq_tuple_ = &tuple;
			const auto& [op, input, output] = *seq_tuple_;
			PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "Handling operation: \"{}\" with input parts [{}] and output parts [{}]",
				op.name(), fmt::join(input, ","), fmt::join(output, ",")));

			std::vector<PlanTransition> plan_transitions;
			auto seq = HandleSequentialOperation(*res_state, plan_transitions, parts_);
			if (!seq.has_value()) {
				return {};
			}
			res_state = seq.value();
		}
		return {res_state};
	}
	
	/**
	 * @brief Handles a single sequential operation. Returns the resulting end-state if reliasable, nullopt otherwise.
	 */
	std::optional<const TopologyState*> Controller::HandleSequentialOperation(const TopologyState& topology_state,
		                                                                     std::vector<PlanTransition> plan_transitions, 
		                                                                     Parts plan_parts) {
		const auto& [op, input, output] = *seq_tuple_;
		// map type - TransferOperation key, tuple<end_state, transition, inverse transition>
		std::unordered_map<TransferOperation, std::tuple<const TopologyState*, const TopologyTransition*, 
			const TopologyTransition*>> transfers;

		for (const auto& transition : topology_->at(topology_state).transitions_) {
			if (op.name() == transition.label().second) {
				parts_ = plan_parts; 
				// bool transfer = TransferParts(transition.label().first, parts_);
				bool transfer = true;
				if (!transfer) {
					return {};
				}
				std::vector<std::string> vec(num_of_resources_, "-");
				vec[transition.label().first] = transition.label().second;

				plan_transitions.emplace_back(&topology_state, vec, &transition.to());
				for (const auto& plan_t : plan_transitions) { // Unpack the plan
					ApplyTransition(plan_t);
				}
				return { &transition.to()};
			} else {
				std::optional<TransferOperation> opt = StringToTransfer(transition.label().second);
				if (opt.has_value()) {
					if (opt->IsOut()) { // Out case
						std::get<0>(transfers[*opt]) = &(transition.to());
						std::get<1>(transfers[*opt]) = &(transition.label());
					}
					else { // In case
						TransferOperation inverse = opt->Inverse(); // The associated map key is the inverse
						std::get<2>(transfers[inverse]) = &(transition.label());
					}
				}
			}
		}

		std::optional<const std::vector<std::string>*> found = std::nullopt;
		for (const auto& [k, v] : transfers) {
			// map type - [ TransferOperation key, tuple<end_state, transition, inverse transition> ]
			const std::vector<std::string>& state_vec = *std::get<0>(v);
			std::vector<std::string> label_vec(num_of_resources_, "-");
			label_vec[std::get<1>(v)->first] = k.name();
			label_vec[std::get<2>(v)->first] = std::get<2>(v)->second;

			plan_transitions.emplace_back(&topology_state, label_vec, &state_vec);
			found = HandleSequentialOperation(state_vec, plan_transitions, plan_parts); // Recurse DFS
			if (found) {
				break;
			}
		}
		return found;
	}


	/*
	 * @brief ApplyTransition will add a single transition to the controller
	 */
	void Controller::ApplyTransition(const PlanTransition& plan_t) {
		controller_.AddTransition(*plan_t.from, plan_t.label, *plan_t.to);
		PCS_INFO(fmt::format(fmt::fg(fmt::color::royal_blue) | fmt::emphasis::bold,
			"Adding controller transition from {} with label ({}) to {}", fmt::join(*plan_t.from, ","), 
			fmt::join(plan_t.label, ","), fmt::join(*plan_t.to, ",")));
	}

}

 /*
	 @Todo: guard operation in each recipe state being handled
 */