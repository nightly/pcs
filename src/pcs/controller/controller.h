#pragma once

#include "pcs/system/system.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"
#include "pcs/operation/transfer.h"

#include <boost/container_hash/hash.hpp>

#include <optional>

namespace pcs {

	class Controller {
	private:
		LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>> controller_;
		const System* machine_;
		const Recipe* recipe_;
		ITopology* topology_;
	
		std::vector<std::vector<std::string>> parts_;
		std::string recipe_state_;
		std::vector<std::string> topology_state_;
		size_t num_of_resources_;

		const std::tuple<Observable, std::vector<std::string>, std::vector<std::string>>* seq_tuple_;
	public:
		Controller(const System* machine, ITopology* topology, const Recipe* recipe);
		std::optional<const LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> Generate();
	private:
		bool HandleComposite(const CompositeOperation& co);
		bool HandleSequentialOperation(const std::vector<std::string>& topology_state, std::vector<std::pair<std::vector<std::string>,
			std::vector<std::string>>> plan_transitions, std::vector<std::vector<std::string>> plan_parts);
		bool ProcessRecipe(const std::string& state);

		void ApplyTransition(const std::pair <std::vector<std::string>, std::vector<std::string>>& transition);
		bool TransferParts(size_t resource, std::vector<std::vector<std::string>>& parts);
	};
}