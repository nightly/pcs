#include "pcs/controller/controller.h"

namespace pcs {

	LabelledTransitionSystem<std::string> GenerateController(const Machine& machine, const Recipe& recipe) {
		LabelledTransitionSystem<std::string> controller;
		const LabelledTransitionSystem<std::string>& topology = machine.GetTopology();
		const std::vector<LabelledTransitionSystem<std::string>>& resources = machine.GetResources();
		controller.SetInitialState(topology.GetInitialState(), true);

		ProcessRecipe(controller, topology, recipe, resources, recipe.lts_.GetInitialState());

		return controller;
	}

	// Recursively process each recipe state until there is none left, also flowing down the correct path based on the guard outcome
	void ProcessRecipe(LabelledTransitionSystem<std::string>& controller, const LabelledTransitionSystem<std::string>& topology, const Recipe& recipe,
	const std::vector<LabelledTransitionSystem<std::string>>& resources, const std::string& current_recipe_state) {
		if (recipe.lts_[current_recipe_state].transitions_.empty()) {
			return;
		}
		const CompositeOperation& co = recipe.lts_[current_recipe_state].transitions_[0].first; // assume 1st transition for guard passing

		const Guard& guard = co.guard; // <- handle guard 

		for (const auto& o : co.parallel) {}

		for (const auto& o : co.sequential) {
			

		}


		ProcessRecipe(controller, topology, recipe, resources, recipe.lts_[current_recipe_state].transitions_[0].second);
	}

}

/* @Todo:
 	1. Topology currently applies all possible transitions, but neglects proper in/out matching during generation

	1. Handle guard failure conditions
*/