#pragma once

#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"

#include <optional>

namespace pcs {

	std::optional<LabelledTransitionSystem<std::string>> GenerateController(const Machine& machine, const Recipe& recipe);

	bool ProcessRecipe(LabelledTransitionSystem<std::string>& controller, const Machine& machine, const Recipe& recipe,
		const std::string& current_recipe_state, std::string& topology_state, std::vector<std::vector<std::string>>& parts);

}