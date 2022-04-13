#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <string>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"

namespace pcs {

	using Topology = LabelledTransitionSystem<std::vector<std::string>, std::string>;

	std::optional<LabelledTransitionSystem<std::string>> GenerateController(const Machine& machine, const Recipe& recipe) {
		LabelledTransitionSystem<std::string> controller;
		std::vector<std::vector<std::string>> ResParts;

		const LabelledTransitionSystem<std::string>& topology = machine.topology();
		std::string initial_state = topology.initial_state();
		controller.set_initial_state(initial_state, true);

		bool successful_generation = ProcessRecipe(controller, machine, recipe, 
			recipe.lts_.initial_state(), initial_state, 0, ResParts);
		
		if (!successful_generation) {
			return std::nullopt;
		}
		return controller;
	}



	// Recursively process each recipe state until there is none left, also flowing down the correct path based on the guard outcome
	// If the recipe cannot be realised at any point, false will be returned
	bool ProcessRecipe(LabelledTransitionSystem<std::string>& controller, const Machine& machine, const Recipe& recipe, 
	const std::string& current_recipe_state, std::string& topology_state, size_t iteration, std::vector<std::vector<std::string>>& ResParts) {
		if (recipe.lts_[current_recipe_state].transitions_.empty()) {
			return true; // No further recipe states to process
		}
		const LabelledTransitionSystem<std::string>& topology = machine.topology();
		const std::vector<LabelledTransitionSystem<std::string>>& resources = machine.resources();
		std::string controller_state = "s" + std::to_string(iteration);
		std::string transition;

		const CompositeOperation& co = recipe.lts_[current_recipe_state].transitions_[0].first; // assume 1st transition as guard passing
		for (const auto& tuple : co.sequential) {
			const auto& [op, input, output] = tuple;
			if (input.empty() || (input == output)) {
				bool found_matching = false;
				for (const auto& t : topology[topology_state].transitions_) {
					if (t.first == op.name_) {
						found_matching = true;
						// Update ResParts with the corresponding resource index
						// ResParts[i] = o.output_;
						topology_state = t.second;
						transition += "[" + topology_state + "]\n";
						break;
					}
				}
				if (!found_matching) {
					return false; // break recursion, as no operation can be found, return nullopt
				}
			}

		}

		++iteration;
		controller.AddTransition(controller_state, transition, "s" + std::to_string(iteration));
		ProcessRecipe(controller, machine, recipe,
			recipe.lts_[current_recipe_state].transitions_[0].second, topology_state, iteration, ResParts);
	}

}

 /*
	 @Todo: separate logic for handling each operation 
     @Todo: guard operations, handle failure case
 */
