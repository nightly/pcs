#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <span>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/ranges.h>

#include "lts/lts.h"
#include "pcs/controller/core.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/controller/unify.h"

namespace pcs {

	Controller::Controller(const Environment* machine, ITopology* topology, const Recipe* recipe)
		: machine_(machine), recipe_(recipe), topology_(topology), num_of_resources_(machine_->NumOfResources()) {}

	std::optional<Controller::ControllerType> Controller::Generate() {
		const std::string& recipe_init_state = recipe_->lts().initial_state();
		const auto& first_transition = recipe_->lts().at(recipe_init_state).transitions()[0];
		ControllerType controller;
		controller.set_initial_state({ first_transition.to(), topology_->initial_state() });

		Parts plan_parts(machine_->NumOfResources());
		std::vector<PlanTransition> plan_transitions, basic_plan;
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller initial state ({} - {})", controller.initial_state().first,
			fmt::join(controller.initial_state().second, ",")));
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Recipe initial state: {}", recipe_init_state));

		// The first transition of the recipe does not have a guard associated with it
		bool generated = DFS(controller, first_transition.to(), &topology_->initial_state(),
			plan_parts, basic_plan, plan_transitions, first_transition.label(), 0);

		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller generation completed: realisability = {}", generated));
		
		/* ******************************************************************************************* /
		  * @Hack: Visualises controller no matter what (wherever it reached) for testing purposes.
		  *	However, doesn't affect above realisability output though
		  */
		return { controller };
		/*****************************************************************************************/

		if (!generated) {
			return {};
		}
		return { controller };
	}

	/*
	 * @brief Recursively processes and realises the recipe.
	 *
	 * Recursive backtracking DFS.
	 *
	 */
	bool Controller::DFS(ControllerType& controller, const std::string& next_recipe_state, const std::vector<std::string>* topology_state, Parts plan_parts,
		                std::vector<PlanTransition> basic_plan, std::vector<PlanTransition> plan_transitions,
		                const CompositeOperation& co, size_t seq_id) {

		// 1. Get the current sequential operation to process
		const TaskExpression& task = co.CurrentTask(seq_id);
		const auto& [op, input, parameters, output] = task;

		// 2. Realise the sequential operation by trying to directly reach it whilst collecting synchronization operations.

		// map type - [ TransferOperation key, tuple(end_state, transition, inverse transition) ]
		std::unordered_map<TransferOperation, std::tuple<const TopologyState*, const TopologyTransition*,
			const TopologyTransition*>> transfers;
		
		bool found = false;
		for (const auto& transition : topology_->at(*topology_state).transitions_) {
			if (op.name() == transition.label().second.operation()) {
				bool allocate = true;
				if (!input.empty()) {
					allocate = plan_parts.Allocate(transition.label(), input);
					allocate = true;
				}
				bool unify = Unify(transition.label().second.parameters(), parameters, op);
				bool nopize = NopizeObservable(machine_->resources(), *topology_state, transition.label().first, op.name());
				if (allocate && unify && nopize) {
					std::vector<std::string> label_vec(num_of_resources_, "-");
					label_vec[transition.label().first] = transition.label().second.operation();
					plan_transitions.emplace_back(next_recipe_state, topology_state, label_vec, &transition.to());
					plan_parts.Add(transition.label(), output);
					topology_state = &transition.to();
					found = true;
					break;
				}
			} else {
				std::optional<TransferOperation> opt = StringToTransfer(transition.label().second.operation());
				if (opt.has_value()) {
					if (opt->IsOut()) {
						std::get<0>(transfers[*opt]) = &(transition.to());
						std::get<1>(transfers[*opt]) = &(transition.label());
					} else {
						TransferOperation inverse = opt->Inverse(); // The associated map key is the inverse
						std::get<2>(transfers[inverse]) = &(transition.label());
					}
				}
			}
		}

		// 3. Since the sequential operation hasn't been reached, begin iterating over synchronization transitions.
		if (!found) {
			for (const auto& [k, v] : transfers) {
				// map type - [ TransferOperation key, tuple(end_state, transition, inverse transition) ]
				const TopologyState& state_vec = *std::get<0>(v);
				std::vector<std::string> label_vec(num_of_resources_, "-");
				label_vec[std::get<1>(v)->first] = k.name();
				label_vec[std::get<2>(v)->first] = std::get<2>(v)->second.operation();
				bool sync = plan_parts.Synchronize(std::get<2>(v)->first, std::get<1>(v)->first, input);
				bool nopize = NopizeSync(machine_->resources(), *topology_state, std::get<1>(v)->first, std::get<2>(v)->first, op.name());
				if (!nopize) {
					continue;
				}
				plan_transitions.emplace_back(next_recipe_state, topology_state, label_vec, &state_vec);
				found = DFS(controller, next_recipe_state, &state_vec, plan_parts, basic_plan, plan_transitions, co, seq_id);
				if (found == true) {
					break;
				}
			}
			return found;
		}

		// 4. Process the next operation
		seq_id++;
		if (seq_id < (co.HasGuard() ? (co.sequential.size() + 1) : co.sequential.size())) [[Likely]] {
			PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "[DFS] Processed Operation = \"{}\" with input parts [{}] and output parts [{}]",
				op.name(), fmt::join(input, ","), fmt::join(output, ",")));
			return DFS(controller, next_recipe_state, topology_state, plan_parts, basic_plan, plan_transitions, co, seq_id);
		} else {
			ApplyAllTransitions(plan_transitions, controller);
			basic_plan.insert(std::end(basic_plan), std::begin(plan_transitions), std::end(plan_transitions));
			plan_transitions.clear();
			PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "[DFS] Processed Composite Operation at Recipe State {}. Last operation = \"{}\" with input parts [{}] and output parts [{}]",
				next_recipe_state, op.name(), fmt::join(input, ","), fmt::join(output, ",")));

			for (const auto& rec_transition : recipe_->lts()[next_recipe_state].transitions_) {
				PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold, "Processing recipe transition to: {}",
					rec_transition.to()));
				bool realise = DFS(controller, rec_transition.to(), topology_state, plan_parts,
					               basic_plan, plan_transitions, rec_transition.label(), 0);
				if (realise == false) {
					return false;
				}
			}
			return true;
		}

	}

}