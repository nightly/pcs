#pragma once

#include "pcs/system/system.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"
#include "pcs/operation/transfer.h"
#include "pcs/controller/plan_transition.h"
#include "pcs/controller/parts.h"

#include <boost/container_hash/hash.hpp>

#include <optional>

namespace pcs {

	class Controller {
	public:
		using TopologyTransition = std::pair<size_t, std::string>;
		using TopologyState = std::vector<std::string>;

		using ControllerTransition = std::vector<std::string>;
		using ControllerState = std::vector<std::string>;
	private:
		LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>> controller_;
		const System* machine_;
		const Recipe* recipe_;
		ITopology* topology_;

		size_t num_of_resources_;

		const std::tuple<Observable, std::vector<std::string>, std::vector<std::string>>* seq_tuple_;
	public:
		Controller(const System* machine, ITopology* topology, const Recipe* recipe);
		std::optional<const LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> Generate();
	private:		

		bool ProcessRecipe(const std::string& recipe_state, const TopologyState* topoloy_state, const Parts plan_parts);

		std::optional<std::pair<const TopologyState*, Parts>> HandleComposite(const CompositeOperation& co, const std::vector<std::string>& topology_state,
			Parts plan_parts);

		std::optional<std::pair<const TopologyState*, Parts>> HandleSequentialOperation(const std::vector<std::string>& topology_state, 
			std::vector<PlanTransition> plan_transitions, Parts plan_parts);

		void ApplyTransition(const PlanTransition& plan_t);
	};
}