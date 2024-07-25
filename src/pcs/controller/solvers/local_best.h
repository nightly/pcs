#pragma once

#include <optional>
#include <span>

#include <boost/container_hash/hash.hpp>

#include "pcs/environment/environment.h"
#include "pcs/product/recipe.h"
#include "lts/lts.h"
#include "pcs/operation/transfer.h"
#include "pcs/controller/plan_transition.h"
#include "pcs/operation/parameterized_op.h"
#include "pcs/controller/parts.h"
#include "pcs/controller/solvers/best.h"

namespace pcs {

	class LocalBestController {
	public:
		using TopologyTransition = std::pair<size_t, ParameterizedOp>;
		using TopologyState = std::vector<std::string>;

		using ControllerTransition = std::vector<std::string>;
		using ControllerState = std::pair<std::string, std::vector<std::string>>;
		using ControllerType = nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
			                                boost::hash<std::pair<std::string, std::vector<std::string>>>>;
		// map type - [ TransferOperation key, tuple(end_state, transition, inverse transition) ]
		using TransferMap = std::unordered_map<TransferOperation, std::tuple<const TopologyState*, const TopologyTransition*, const TopologyTransition*>>;
	private:
		const Environment* machine_;
		const Recipe* recipe_;
		ITopology* topology_;

		size_t num_of_resources_;		
		MinimizeOpt opt_;
		std::vector<double> costs_;

		std::unordered_set<size_t> used_resources_;
		std::list<size_t> list_used_resources_;
		size_t cost_;

		struct LocalCandidate {
		public:
			using TopologyState = std::vector<std::string>;

			TopologyState state_vec_;
			Parts next_parts_;
			std::vector<PlanTransition> next_transitions_;
			size_t cost_; // number of resources used or their cost
			std::unordered_set<size_t> used_resources_; // If minimizing the number of resources, this is used.
			std::list<size_t> list_used_resources_;
		public:
			LocalCandidate()
				: cost_(0)
			{}

			LocalCandidate(const TopologyState& state_vec, const Parts& next_parts, const std::vector<PlanTransition>& next_transitions,
				const std::unordered_set<size_t>& used_resources, const std::list<size_t>& list_used_resources, size_t cost)
				: state_vec_(state_vec), next_parts_(next_parts), next_transitions_(next_transitions), used_resources_(used_resources), list_used_resources_(list_used_resources), cost_(cost)
			{}
		};

		// Min order comparator for LocalCandidates
		struct LocalCandidateComparator {
		public:
			LocalCandidateComparator() {}

			bool operator () (const LocalCandidate& a, const LocalCandidate& b) {
				return a.cost_ > b.cost_;
			}
		};
	public:
		LocalBestController(const Environment* machine, ITopology* topology, const Recipe* recipe);
		std::optional<ControllerType> Generate(MinimizeOpt opt, std::optional<std::filesystem::path> costs_path = std::nullopt);
		size_t GetCost();
		std::unordered_set<size_t> GetResources();
		std::list<size_t> GetResourceList();
	private:
		bool Advance(const TaskExpression& task, std::vector<PlanTransition>& plan_transitions, Parts& plan_parts, const std::string& next_recipe_state,
			const std::vector<std::string>*& topology_state, TransferMap& transfers, MinimizeOpt opt,
			std::unordered_set<size_t>& used_resources, std::list<size_t>& list_used_resources, size_t& cost);
		bool DFS(ControllerType& controller, const std::string& recipe_state, const std::vector<std::string>* topology_state,
			Parts plan_parts, std::vector<PlanTransition> basic_plan, std::vector<PlanTransition> plan_transitions, const CompositeOperation& co, size_t seq_id,
			std::unordered_set<size_t>& used_resources, std::list<size_t>& list_used_resources, size_t& cost, size_t recursion_level);
		void SetCosts(std::optional<std::filesystem::path> path);
		void UpdateCost(LocalCandidate& cand, const TopologyTransition& transition, MinimizeOpt opt);
		void UpdateCost(std::unordered_set<size_t>& used_resources, std::list<size_t>& list_used_resources, size_t& cost, const TopologyTransition& transition, MinimizeOpt opt);
	};
}