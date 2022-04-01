#pragma once

#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"

namespace pcs {

	LabelledTransitionSystem<std::string> GenerateController(const Machine& machine, const Recipe& recipe);

	void ProcessRecipe(LabelledTransitionSystem<std::string>& controller, const LabelledTransitionSystem<std::string>& topology, const Recipe& recipe,
	const std::vector<LabelledTransitionSystem<std::string>>& resources, const std::string& current_recipe_state);

}