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
		: machine_(machine), recipe_(recipe), topology_(topology), num_of_resources_(machine_->NumOfResources()) {
	}

	struct Stage {
		const TopologyState* topology_state;
		Parts parts;
		const std::string* start_recipe_state;
		const std::string* to_recipe_state;
		size_t seq_id;
	public:
		Stage() = delete;
		
		Stage(const TopologyState& topology_state, const Parts& parts, 
			  const std::string& start_recipe_state, const std::string& to_recipe_state, size_t seq_id)
		: topology_state(&topology_state), parts(parts), start_recipe_state(&start_recipe_state), 
		  to_recipe_state(&to_recipe_state), seq_id(seq_id) {}

	};

	struct Candidate {
	public:
		ControllerType controller;
		size_t num; // number of either resources or synchronization transitions used
		size_t complete_ops; // tracks how many recipe operations a candidate has completed
		std::queue<Stage> descendants; // what needs to be processed. A controller is complete when there are no more descendants to process.
	
		std::unordered_set<size_t> used_resources; // If minimizing the number of resources, this is used.
	public:
		Candidate() = default;

		Candidate(const Stage& descendant)
			: num(0) {
			descendants.push(descendant);
		}

		Candidate(const ControllerType& controller, size_t num, const std::queue<Stage>& descendants, size_t seq_id)
			: controller(controller), num(num), descendants(descendants) {}
	};

	// Min order comparator for Candidates
	struct CandidateComparator {
	public:
		CandidateComparator() {}

		bool operator () (const Candidate& a, const Candidate& b) {
			return a.num > b.num;
		}
	};

	const CompositeOperation& GetComposite(const Stage& stage, const Recipe& recipe) {
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

	void BestController::CostsUpdate(MinimizeOpt opt, std::unordered_set<size_t>& used_resources,
		                            const TopologyTransition& transition, size_t& num) {
		switch (opt) {
		case MinimizeOpt::Transitions:
			num++;
			break;
		case MinimizeOpt::Resources:
			if (!used_resources.contains(transition.first)) {
				used_resources.emplace(transition.first);
				num++;
			}
			break;
		case MinimizeOpt::Cost:
			if (!used_resources.contains(transition.first)) {
				used_resources.emplace(transition.first);
				num+= costs_[transition.first];
			}
			break;
		}
	}


	std::optional<ControllerType> BestController::Generate(MinimizeOpt opt, std::optional<std::filesystem::path> costs_path) {
		bool first_generated = false;
		Candidate best_candidate;
		best_candidate.num = std::numeric_limits<size_t>::max();
		std::priority_queue<Candidate, std::vector<Candidate>, CandidateComparator> pq;

		Parts initial_parts(num_of_resources_);
		const std::string& recipe_init_state = recipe_->lts().initial_state();
		const auto& first_transition = recipe_->lts().at(recipe_init_state).transitions()[0];
		Stage initial_descendant(topology_->initial_state(), initial_parts, recipe_init_state, first_transition.to(), 0);
		Candidate initial_candidate(initial_descendant);
		initial_candidate.controller.set_initial_state({first_transition.to(), topology_->initial_state()});
		pq.push(initial_candidate);

		if (opt == MinimizeOpt::Cost) {
			SetCosts(costs_path);
			if (costs_.empty()) {
				return {};
			}
		}

		while (!pq.empty() && (best_candidate.num >= pq.top().num || !first_generated)) {
			 Candidate cand = pq.top();
			 pq.pop();

			 Stage& stage = cand.descendants.front();
			 const CompositeOperation& co = GetComposite(stage, *recipe_);
			 const TaskExpression& task = co.CurrentTask(stage.seq_id);
			 const auto& [op, input, parameters, output] = task;

			 std::unordered_map<TransferOperation, std::tuple<const TopologyState*, const TopologyTransition*,
				 const TopologyTransition*>> transfers;

			 bool found = false;
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
						 found = true;

						 CostsUpdate(opt, cand.used_resources, transition.label(), cand.num);

						 cand.complete_ops++;
						 break;
					 }
				 } else {
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

					 CostsUpdate(opt, next_candidate.used_resources, *std::get<1>(v), next_candidate.num);
					 
					 pq.push(next_candidate);
				 }
				 continue;
			 } 

			 // Operation was found

			 if (cand.num < best_candidate.num || cand.complete_ops > best_candidate.complete_ops) {
				 best_candidate = cand;
			 }

			 stage.seq_id++;
			 if (stage.seq_id < (co.HasGuard() ? (co.sequential.size() + 1) : co.sequential.size())) [[Likely]] { // Process next sequential operation
				 pq.push(cand);
			 } else { // Process recipe transitions
				 for (const auto& rec_transition : recipe_->lts()[*stage.to_recipe_state].transitions_) { 
					 Stage next_stage(*stage.topology_state, stage.parts, *stage.to_recipe_state, rec_transition.to(), 0);
					 cand.descendants.push(next_stage);
				 }
				 cand.descendants.pop();
				 if (cand.descendants.empty()) {
					 first_generated = true;
				 } else {
					 pq.push(cand);
				 }
			 }
		}

		PCS_INFO(fmt::format(fmt::fg(fmt::color::gold) | fmt::emphasis::bold,
			"Final cost = {}", best_candidate.num));

#ifdef PRINT_COST
		std::cout << "Final cost = {" + std::to_string(best_candidate.num) + "}" << std::endl;
#endif

		return { best_candidate.controller };
	}

}
