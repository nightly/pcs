#pragma once

#include <optional>
#include <span>
#include <string>
#include <vector>
#include <queue>
#include <filesystem>

#include <boost/container_hash/hash.hpp>

#include "pcs/environment/environment.h"
#include "pcs/product/recipe.h"
#include "lts/lts.h"
#include "pcs/operation/transfer.h"
#include "pcs/controller/plan_transition.h"
#include "pcs/operation/parameterized_op.h"
#include "pcs/controller/parts.h"

namespace pcs {

	/*
	 * @brief Whether the best controller solution should minimize the number of transitions or resources
	 */
	enum class MinimizeOpt {
		Transitions,
		Resources,
		Cost,
		CostEstimate
		// Multi Objective
	};

	class BestController {
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
		size_t composite_ops_;
		ITopology* topology_;

		size_t num_of_resources_;
		std::vector<double> costs_;

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
			size_t path_cost; // cost of the current controller (used by A*)
			size_t cost; // (estimated) cost of the (complete) controller
			size_t complete_ops; // tracks how many recipe operations a candidate has completed
			size_t complete_composite_ops; // tracks how many recipe composite operations a candidate has completed
			std::queue<Stage> descendants; // what needs to be processed. A controller is complete when there are no more descendants to process.

			std::unordered_set<size_t> used_resources; // If minimizing the number of resources, this is used.
		public:
			Candidate() = default;

			Candidate(const Stage& descendant)
				: path_cost(0), cost(0), complete_ops(0), complete_composite_ops(0) {
				descendants.push(descendant);
			}
		};

		// Min order comparator for Candidates
		struct CandidateComparator {
		public:
			CandidateComparator() {}

			bool operator () (const Candidate& a, const Candidate& b) {
				return a.cost > b.cost;
			}
		};
	public:
		BestController(const Environment* machine, ITopology* topology, const Recipe* recipe);
		std::optional<ControllerType> Generate(MinimizeOpt opt, std::optional<std::filesystem::path> costs_path = std::nullopt);
	private:
		void SetCosts(std::optional<std::filesystem::path> path);
		void UpdateCost(Candidate& cand, MinimizeOpt opt, const TopologyTransition& transition);
		const CompositeOperation& GetComposite(const Stage& stage, const Recipe& recipe);
	};

}