#pragma once

#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"
#include "pcs/operation/transfer.h"

#include <optional>

namespace pcs {

	class Controller {
	private:
		LTS<std::string, std::string> controller_;
		const Machine* machine_;
		const Recipe* recipe_;
		const LTS<std::string, std::string>* topology_;

		std::vector<std::vector<std::string>> parts_;
		std::string recipe_state_;
		std::string topology_state_;
		size_t num_of_resources_;

		const std::tuple<Operation, std::vector<std::string>, std::vector<std::string>>* seq_tuple_;
	public:
		Controller(const Machine* machine, const Recipe* recipe);
		std::optional<const LTS<std::string, std::string>*> Generate();
	private:
		bool HandleComposite(const CompositeOperation& co);
		bool HandleSequentialOperation(std::string topology_state,
			std::vector<std::pair<std::vector<std::string>, std::string>> plan_transitions, std::vector<std::vector<std::string>> plan_parts);
		bool ProcessRecipe(const std::string& state);

		void ApplyTransition(const std::pair<std::vector<std::string>, std::string>& transition);
		bool TransferParts(const std::pair<std::string, std::string>& transition, size_t current_resource);
	};
}