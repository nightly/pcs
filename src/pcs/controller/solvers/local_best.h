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
	private:
		const Environment* machine_;
		const Recipe* recipe_;
		ITopology* topology_;

		size_t num_of_resources_;		
		MinimizeOpt opt_;
		std::vector<double> costs_;
		std::unordered_set<size_t> used_resources_;
		size_t final_cost_;
	public:
		LocalBestController(const Environment* machine, ITopology* topology, const Recipe* recipe);
		std::optional<ControllerType> Generate(MinimizeOpt opt, std::optional<std::filesystem::path> costs_path = std::nullopt);
	private:
		bool DFS(ControllerType& controller, const std::string& recipe_state, const std::vector<std::string>* topology_state, Parts plan_parts,
			std::vector<PlanTransition> basic_plan, std::vector<PlanTransition> plan_transitions,
			const CompositeOperation& co, size_t seq_id, std::unordered_set<size_t> used_resources, size_t cost, size_t recursion_level = 0);
		void SetCosts(std::optional<std::filesystem::path> path);
		void UpdateCost(MinimizeOpt opt, std::unordered_set<size_t>& used_resources, const TopologyTransition& transition, size_t& cost);
		void UpdateCost(MinimizeOpt opt, std::unordered_set<size_t>& used_resources, const std::unordered_set<size_t>& resources, size_t& cost);
	};
}