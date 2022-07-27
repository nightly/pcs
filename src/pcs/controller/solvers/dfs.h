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

namespace pcs {

	class Controller {
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
	public:
		Controller(const Environment* machine, ITopology* topology, const Recipe* recipe);
		std::optional<ControllerType> Generate();
	private:
		bool DFS(ControllerType& controller, const std::string& recipe_state, const std::vector<std::string>* topology_state, Parts plan_parts,
			std::vector<PlanTransition> basic_plan, std::vector<PlanTransition> plan_transitions, const CompositeOperation& co, size_t seq_id);
	};
}