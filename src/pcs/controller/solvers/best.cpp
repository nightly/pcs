#include "pcs/controller/solvers/best.h"

#include <optional>
#include <vector>
#include <span>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>
#include <stdexcept>

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

	using TopologyTransition = std::pair<size_t, ParameterizedOp>;
	using TopologyState = std::vector<std::string>;

	using ControllerTransition = std::vector<std::string>;
	using ControllerState = std::pair<std::string, std::vector<std::string>>;
	using ControllerType = nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
		                       boost::hash<std::pair<std::string, std::vector<std::string>>>>;

	BestController::BestController(const Environment* machine, ITopology* topology, const Recipe* recipe) 
		: machine_(machine), recipe_(recipe), composite_ops_(recipe->lts().NumOfTransitions()), topology_(topology), num_of_resources_(machine_->NumOfResources()) {
	}

	const CompositeOperation& BestController::GetComposite(const Stage& stage, const Recipe& recipe) {
		for (const auto& transition : recipe.lts().at(*stage.start_recipe_state).transitions()) {
			if (transition.to() == *stage.to_recipe_state) {
				return transition.label();
			}
		}
	}

	void BestController::SetCosts(std::optional<std::filesystem::path> path) {
		if (path.has_value()) {
			try {
				costs_ = ParseCosts(path.value(), num_of_resources_);
			} catch (std::ifstream::failure& e) {
				throw;
			}
			PCS_INFO(fmt::format(fmt::fg(fmt::color::white_smoke) | fmt::emphasis::bold,
				"Using cost objective with the following costs [{}]", fmt::join(costs_, ",")));
		} else {			
			PCS_CRITICAL("Specified to use cost objective but did not provide any filepath to costs");
		}
	}

	void BestController::UpdateCost(Candidate& cand, MinimizeOpt opt, const TopologyTransition& transition) {
		switch (opt) {
		case MinimizeOpt::Transitions:
			cand.cost++;
			break;
		case MinimizeOpt::Resources:
			if (!cand.used_resources.contains(transition.first)) {
				cand.used_resources.emplace(transition.first);
				cand.cost++;
			}
			break;
		case MinimizeOpt::Cost:
			cand.cost += costs_[transition.first];
			cand.list_used_resources.emplace_back(transition.first);
			break;
		case MinimizeOpt::CostEstimate:
			cand.path_cost += costs_[transition.first];
			cand.cost = cand.path_cost + composite_ops_ - cand.complete_composite_ops;
			cand.list_used_resources.emplace_back(transition.first);
			break;
		default:
			assert(false);
		}
	}

	void BestController::UpdateCost(Candidate& cand, MinimizeOpt opt) {
		if (opt == MinimizeOpt::CostEstimate) {
			cand.cost = cand.path_cost + composite_ops_ - cand.complete_composite_ops;
		}
	}

	bool BestController::AdvanceStage(Candidate& cand, TransferMap& transfers, MinimizeOpt opt)
	{
		bool found = false;

		Stage& stage = cand.descendants.front();
		const CompositeOperation& co = GetComposite(stage, *recipe_);
		const TaskExpression& task = co.CurrentTask(stage.seq_id);
		const auto& [op, input, parameters, output] = task;

		for (const auto& transition : topology_->at(*stage.topology_state).transitions_) {
			if (op.name() == transition.label().second.operation()) {
				bool allocate = true;
				if (!input.empty()) {
					allocate = stage.parts.Allocate(transition.label(), input);
					allocate = true;
				}
				bool unify = Unify(transition.label().second.parameters(), parameters, op);
				bool nopize = NopizeObservable(machine_->resources(), *stage.topology_state, transition.label().first, op.name());
				if (allocate && unify && nopize) {
					std::vector<std::string> label_vec(num_of_resources_, "-");
					label_vec[transition.label().first] = transition.label().second.operation();

					PlanTransition plan_t(*stage.to_recipe_state, stage.topology_state, label_vec, &transition.to());
					ApplyTransition(plan_t, cand.controller);

					stage.parts.Add(transition.label(), output);
					stage.topology_state = &transition.to();

					cand.complete_ops++;
					UpdateCost(cand, opt, transition.label());

					found = true;
					break;
				}
			}
			else {
				std::optional<TransferOperation> opt = StringToTransfer(transition.label().second.operation());
				if (opt.has_value()) {
					if (opt->IsOut()) {
						std::get<0>(transfers[*opt]) = &(transition.to());
						std::get<1>(transfers[*opt]) = &(transition.label());
					}
					else {
						TransferOperation inverse = opt->Inverse(); // The associated map key is the inverse
						std::get<2>(transfers[inverse]) = &(transition.label());
					}
				}
			}
		}

		return found;
	}

	std::optional<ControllerType> BestController::Generate(MinimizeOpt opt, std::optional<std::filesystem::path> costs_path) {
		Candidate best_candidate;
		std::priority_queue<Candidate, std::vector<Candidate>, CandidateComparator> pq;

		Parts initial_parts(num_of_resources_);
		const std::string& recipe_init_state = recipe_->lts().initial_state();
		const auto& first_transition = recipe_->lts().at(recipe_init_state).transitions()[0];
		Stage initial_descendant(topology_->initial_state(), initial_parts, recipe_init_state, first_transition.to(), 0);
		Candidate initial_candidate(initial_descendant);
		initial_candidate.controller.set_initial_state({ first_transition.to(), topology_->initial_state() });
		pq.push(initial_candidate);

		if (opt == MinimizeOpt::Cost || opt == MinimizeOpt::CostEstimate) {
			SetCosts(costs_path);
			if (costs_.empty()) {
				return {};
			}
		}

		while (!pq.empty()) {
			Candidate cand = pq.top();
			pq.pop();

			if (cand.descendants.empty()) { // Found solution
				best_candidate = cand;
				break;
			}

			Stage& stage = cand.descendants.front();
			const CompositeOperation& co = GetComposite(stage, *recipe_);
			const TaskExpression& task = co.CurrentTask(stage.seq_id);
			const auto& [op, input, parameters, output] = task;
			TransferMap transfers;

			bool found = AdvanceStage(cand, transfers, opt);

			// Handle case where the target operation is not found
			if (!found) {
				for (const auto& [k, v] : transfers) {
					Candidate next_candidate = cand;
					Stage& next_stage = next_candidate.descendants.front();

					// map type - [ TransferOperation key, tuple(end_state, transition, inverse transition) ]
					const TopologyState& state_vec = *std::get<0>(v);
					std::vector<std::string> label_vec(num_of_resources_, "-");
					label_vec[std::get<1>(v)->first] = k.name();
					label_vec[std::get<2>(v)->first] = std::get<2>(v)->second.operation();
					bool sync = next_stage.parts.Synchronize(std::get<2>(v)->first, std::get<1>(v)->first, input);
					bool nopize = NopizeSync(machine_->resources(), *next_stage.topology_state, std::get<1>(v)->first, std::get<2>(v)->first, op.name());
					if (!nopize || !sync) {
						continue;
					}

					PlanTransition plan_t(*next_stage.to_recipe_state, next_stage.topology_state, label_vec, &state_vec);
					next_stage.topology_state = &state_vec;
					ApplyTransition(plan_t, next_candidate.controller);

					UpdateCost(next_candidate, opt, *std::get<2>(v));

					pq.push(next_candidate);
				}
				continue;
			}

			// Operation was found			
			stage.seq_id++;			
			if (stage.seq_id >= (co.HasGuard() ? (co.sequential.size() + 1) : co.sequential.size())) [[Likely]] {
				// Composite operation complete, process recipe transitions
				cand.complete_composite_ops++;
				UpdateCost(cand, opt);
				for (const auto& rec_transition : recipe_->lts()[*stage.to_recipe_state].transitions_) {
					Stage next_stage(*stage.topology_state, stage.parts, *stage.to_recipe_state, rec_transition.to(), 0);
					cand.descendants.push(next_stage);
				}
				cand.descendants.pop();
			}

			pq.push(cand);
		}

		if (opt == MinimizeOpt::CostEstimate) {
			PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold,
				"Final estimated cost = {}", best_candidate.cost));
			PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold,
				"Final actual cost = {}", best_candidate.path_cost));
		}
		else {
			PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold,
				"Final cost = {}", best_candidate.cost));
		}

#ifdef PRINT_COST
		if (opt == MinimizeOpt::CostEstimate) {
			std::cout << "Final estimated cost = {" + std::to_string(best_candidate.cost) + "}" << std::endl;
			std::cout << "Final actual cost = {" + std::to_string(best_candidate.path_cost) + "}" << std::endl;
		}
		else {
			std::cout << "Final cost = {" + std::to_string(best_candidate.cost) + "}" << std::endl;
		}
#endif

		best_candidate_ = best_candidate;

		return { best_candidate.controller };
	}

	size_t BestController::GetCost()
	{
		return best_candidate_.cost;
	}

	std::unordered_set<size_t> BestController::GetResources()
	{
		return best_candidate_.used_resources;
	}

	std::list<size_t> BestController::GetResourceList()
	{
		return best_candidate_.list_used_resources;
	}

}
