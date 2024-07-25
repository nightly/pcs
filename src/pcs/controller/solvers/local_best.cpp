#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <span>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cassert>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/ranges.h>

#include "lts/lts.h"
#include "pcs/controller/core.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/controller/unify.h"
#include "pcs/controller/solvers/parse_costs.h"

namespace pcs {

	LocalBestController::LocalBestController(const Environment* machine, ITopology* topology, const Recipe* recipe)
		: machine_(machine), recipe_(recipe), topology_(topology), num_of_resources_(machine_->NumOfResources()), used_resources_(), list_used_resources_(), cost_(0) {}

	void LocalBestController::SetCosts(std::optional<std::filesystem::path> path) {
		if (path.has_value()) {
			try {
				costs_ = ParseCosts(path.value(), num_of_resources_);
			}
			catch (std::ifstream::failure& e) {
				throw;
			}
			PCS_INFO(fmt::format(fmt::fg(fmt::color::white_smoke) | fmt::emphasis::bold,
				"Using cost objective with the following costs [{}]", fmt::join(costs_, ",")));
		}
		else {
			PCS_CRITICAL("Specified to use cost objective but did not provide any filepath to costs");
		}
	}

	void LocalBestController::UpdateCost(LocalCandidate& cand, const TopologyTransition& transition, MinimizeOpt opt) {
		UpdateCost(cand.used_resources_, cand.list_used_resources_, cand.cost_, transition, opt);
	}

	void LocalBestController::UpdateCost(std::unordered_set<size_t>& used_resources, std::list<size_t>& list_used_resources, size_t& cost, const TopologyTransition& transition, MinimizeOpt opt) {
		switch (opt) {
		case MinimizeOpt::Resources:
			if (!used_resources.contains(transition.first)) {
				used_resources.emplace(transition.first);
				cost++;
			}			
			break;
		case MinimizeOpt::Cost:
			list_used_resources.emplace_back(transition.first);
			cost += costs_[transition.first];
			break;
		default:
			assert(false);
		}
	}

	std::optional<Controller::ControllerType> LocalBestController::Generate(MinimizeOpt opt, std::optional<std::filesystem::path> costs_path) {
		const std::string& recipe_init_state = recipe_->lts().initial_state();
		const auto& first_transition = recipe_->lts().at(recipe_init_state).transitions()[0];
		ControllerType controller;
		controller.set_initial_state({ first_transition.to(), topology_->initial_state() });

		Parts plan_parts(machine_->NumOfResources());
		std::vector<PlanTransition> plan_transitions, basic_plan;
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller initial state ({} - {})", controller.initial_state().first,
			fmt::join(controller.initial_state().second, ",")));
		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Recipe initial state: {}", recipe_init_state));

		opt_ = opt;
		if (opt_ == MinimizeOpt::Cost) {
			SetCosts(costs_path);
			if (costs_.empty()) {
				return {};
			}
		}

		used_resources_ = std::unordered_set<size_t>();
		list_used_resources_ = std::list<size_t>();
		cost_ = 0;

		// The first transition of the recipe does not have a guard associated with it
		bool generated = DFS(controller, first_transition.to(), &topology_->initial_state(), plan_parts, basic_plan, plan_transitions, first_transition.label(), 0,
			used_resources_, list_used_resources_, cost_, 0);

		PCS_INFO(fmt::format(fmt::fg(fmt::color::light_green), "Controller generation completed: realisability = {}", generated));

#ifdef PRINT_COST
		if (opt_ == MinimizeOpt::Resources) {
			std::cout << "Final cost = {" + std::to_string(final_cost_) + "}" << std::endl;

			std::cout << "Used resources = {";
			for (auto it = used_resources_.begin(); it != used_resources_.end(); ++it)
			{
				std::cout << *it << " ";
			}
			std::cout << "}" << std::endl;
		} else if (opt_ == MinimizeOpt::Cost) {
			std::cout << "Final cost = {" + std::to_string(final_cost_) + "}" << std::endl;

			std::cout << "List used resources = {";
			for (auto it = list_used_resources_.begin(); it != list_used_resources_.end(); ++it)
			{
				std::cout << *it << " ";
			}
			std::cout << "}" << std::endl;
		}
#endif
		
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

	bool LocalBestController::Advance(const TaskExpression& task, std::vector<PlanTransition>& plan_transitions, Parts& plan_parts, const std::string& next_recipe_state,
			const std::vector<std::string>*& topology_state, TransferMap& transfers, MinimizeOpt opt,
			std::unordered_set<size_t>& used_resources, std::list<size_t>& list_used_resources, size_t& cost) {
		bool found = false;

		const auto& [op, input, parameters, output] = task;

		for (const auto& transition : topology_->at(*topology_state).transitions_) {
			if (op.name() == transition.label().second.operation()) {
				bool allocate = true;
				if (!input.empty()) {
					allocate = plan_parts.Allocate(transition.label(), input);
				}
				bool unify = Unify(transition.label().second.parameters(), parameters, op);
				bool nopize = NopizeObservable(machine_->resources(), *topology_state, transition.label().first, op.name());
				if (allocate && unify && nopize) {
					std::vector<std::string> label_vec(num_of_resources_, "-");
					label_vec[transition.label().first] = transition.label().second.operation();

					PCS_TRACE(fmt::format(fmt::fg(fmt::color::royal_blue) | fmt::emphasis::bold,
						"Evaluating controller transition from {} with label ({}) to {}", fmt::join(*topology_state, ","),
						fmt::join(label_vec, ","), fmt::join(transition.to(), ",")));

					plan_transitions.emplace_back(next_recipe_state, topology_state, label_vec, &transition.to());
					plan_parts.Add(transition.label(), output);
					topology_state = &transition.to();
					UpdateCost(used_resources, list_used_resources, cost, transition.label(), opt_);
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

		return found;
	}

	/*
	 * @brief Recursively processes and realises the recipe.
	 *
	 * Recursive backtracking DFS.
	 *
	 */
	bool LocalBestController::DFS(ControllerType& controller, const std::string& next_recipe_state, const std::vector<std::string>* topology_state,
			Parts plan_parts, std::vector<PlanTransition> basic_plan, std::vector<PlanTransition> plan_transitions,	const CompositeOperation& co, size_t seq_id, 
			std::unordered_set<size_t>& used_resources, std::list<size_t>& list_used_resources, size_t& cost, size_t recursion_level) {

		// 1. Get the current sequential operation to process
		const TaskExpression& task = co.CurrentTask(seq_id);
		const auto& [op, input, parameters, output] = task;

		// 2. Realise the sequential operation by trying to directly reach it whilst collecting synchronization operations.

		TransferMap transfers;
		bool found = Advance(task, plan_transitions, plan_parts, next_recipe_state, topology_state, transfers, opt_, used_resources, list_used_resources, cost);
		
		// 3. Since the sequential operation hasn't been reached, begin iterating over synchronization transitions.
		if (!found) {
			std::priority_queue<LocalCandidate, std::vector<LocalCandidate>, LocalCandidateComparator> pq;

			for (const auto& [k, v] : transfers) {
				// map type - [ TransferOperation key, tuple(end_state, transition, inverse transition) ]
				// std::get<2>(v) {in} // std::get<1>(v) {out}
				const TopologyState& state_vec = *std::get<0>(v);
				std::vector<std::string> label_vec(num_of_resources_, "-");
				label_vec[std::get<1>(v)->first] = k.name();
				label_vec[std::get<2>(v)->first] = std::get<2>(v)->second.operation();
				Parts next_parts = plan_parts;
				std::vector<PlanTransition> next_transitions = plan_transitions;
				bool sync = next_parts.Synchronize(std::get<2>(v)->first, std::get<1>(v)->first, input);
				bool nopize = NopizeSync(machine_->resources(), *topology_state, std::get<1>(v)->first, std::get<2>(v)->first, op.name());

				PCS_TRACE("//---------------------");
				PCS_TRACE(next_parts);
				PCS_TRACE(fmt::format(fmt::fg(fmt::color::royal_blue) | fmt::emphasis::bold,
					"Evaluating controller transition from {} with label ({}) to {}. For {}. At recursion level = {}", fmt::join(*topology_state, ","),
					fmt::join(label_vec, ","), fmt::join(state_vec, ","), op.name(), recursion_level));
				PCS_TRACE(next_parts);
				PCS_TRACE("---------------------//");
				
				if (!nopize || !sync) {
					continue;
				}
				next_transitions.emplace_back(next_recipe_state, topology_state, label_vec, &state_vec);
				
				LocalCandidate cand(state_vec, next_parts, next_transitions, used_resources, list_used_resources, cost);
				UpdateCost(cand, *std::get<2>(v), opt_);
				pq.push(cand);
			}

			while (!pq.empty()) {
				LocalCandidate cand = pq.top();
				pq.pop();				
				found = DFS(controller, next_recipe_state, &cand.state_vec_, std::move(cand.next_parts_), basic_plan, std::move(cand.next_transitions_), co, seq_id,
					cand.used_resources_, cand.list_used_resources_, cand.cost_, ++recursion_level);
				if (found) {
					used_resources = cand.used_resources_;
					list_used_resources = cand.list_used_resources_;
					cost = cand.cost_;
					return true;
				}
			}

			return found;
		} else {
			// 4. Process the next operation
			seq_id++;
			if (seq_id < (co.HasGuard() ? (co.sequential.size() + 1) : co.sequential.size())) [[Likely]] {
				PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "[DFS] Processed Operation = \"{}\" with input parts [{}] and output parts [{}]",
					op.name(), fmt::join(input, ","), fmt::join(output, ",")));
				return DFS(controller, next_recipe_state, topology_state, plan_parts, basic_plan, plan_transitions, co, seq_id,
					used_resources, list_used_resources, cost, ++recursion_level);
			} else {
				ControllerType rec_controller = controller;
				ApplyAllTransitions(plan_transitions, rec_controller);
				std::vector<PlanTransition> rec_basic_plan = basic_plan;
				rec_basic_plan.insert(std::end(rec_basic_plan), std::begin(plan_transitions), std::end(plan_transitions));
				plan_transitions.clear();
				PCS_INFO(fmt::format(fmt::fg(fmt::color::lavender), "[DFS] Processed Composite Operation at Recipe State {}. Last operation = \"{}\" with input parts [{}] and output parts [{}]",
					next_recipe_state, op.name(), fmt::join(input, ","), fmt::join(output, ",")));

				std::unordered_set<size_t> rec_used_resources = used_resources;
				std::list<size_t> rec_list_used_resources = list_used_resources;
				size_t rec_cost = cost;

				for (const auto& rec_transition : recipe_->lts()[next_recipe_state].transitions_) {
					PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold, "Processing recipe transition to: {}",
						rec_transition.to()));
					bool realise = DFS(rec_controller, rec_transition.to(), topology_state, plan_parts, rec_basic_plan, plan_transitions, rec_transition.label(), 0,
						rec_used_resources, rec_list_used_resources, rec_cost, ++recursion_level);
					if (realise == false) {
						return false;
					}
				}

				controller = rec_controller;
				basic_plan = rec_basic_plan;
				used_resources = rec_used_resources;
				list_used_resources = rec_list_used_resources;
				cost = rec_cost;
				return true;
			}
		}
	}

	size_t LocalBestController::GetCost()
	{
		return cost_;
	}

	std::unordered_set<size_t> LocalBestController::GetResources()
	{
		return used_resources_;
	}

	std::list<size_t> LocalBestController::GetResourceList()
	{
		return list_used_resources_;
	}

}