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
		LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>> controller_;
		const System* machine_;
		const Recipe* recipe_;
		ITopology* topology_;
	
		std::vector<std::vector<std::string>> parts_;
		std::string recipe_state_;
		size_t num_of_resources_;

		const std::tuple<Observable, std::vector<std::string>, std::vector<std::string>>* seq_tuple_;
	public:
		Controller(const System* machine, ITopology* topology, const Recipe* recipe);
		std::optional<const LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> Generate();
	private:		
		bool ProcessRecipe(const std::string& recipe_state, const std::vector<std::string>* topoloy_state);
		std::optional<const std::vector<std::string>*> HandleComposite(const CompositeOperation& co, const std::vector<std::string>& topology_state);
		std::optional<const std::vector<std::string>*> HandleSequentialOperation(const std::vector<std::string>& topology_state, std::vector<std::pair<std::vector<std::string>,
			std::vector<std::string>>> plan_transitions, std::vector<std::vector<std::string>> plan_parts);

		void ApplyTransition(const std::vector<std::string>& from, const std::vector<std::string>& transition, const std::vector<std::string>& to);
		bool TransferParts(size_t resource, std::vector<std::vector<std::string>>& parts);
	};
}