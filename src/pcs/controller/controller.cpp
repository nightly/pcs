#include "pcs/controller/controller.h"

#include <optional>
#include <vector>
#include <string>
#include <algorithm>

#include <spdlog/fmt/ranges.h>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"

namespace pcs {

	using Topology = LTS<std::vector<std::string>, std::string>;

	std::optional<LTS<std::string, std::string>> GenerateController(const Machine& machine, const Recipe& recipe) {
		LTS<std::string, std::string> controller;
		std::vector<std::vector<std::string>> parts;
		parts.resize(machine.NumOfResources());

		const LTS<std::string, std::string>& topology = machine.topology();
		std::string initial_state = topology.initial_state();
		controller.set_initial_state(initial_state, true);
		PCS_INFO("Controller initial state {}", controller.initial_state());
		PCS_INFO("Recipe initial state: {}", recipe.lts().initial_state());
		bool successful_generation = ProcessRecipe(controller, machine, recipe,
			recipe.lts().initial_state(), initial_state, parts);

		if (!successful_generation) {
			return {};
		}
		return controller;
	}

	// Identifies current resource being reviewed based on old topology string state and new string state
	static size_t CurrentResource(const std::string& old_state, const std::string& new_state) {
		std::vector<std::string> old_ve = StringToVector(old_state);
		std::vector<std::string> new_ve = StringToVector(new_state);
		for (size_t i = 0; i < new_ve.size(); ++i) {
			if (old_ve[i] != new_ve[i])
				return i;
		}
	}

	static bool HandleComposite(LTS<std::string, std::string>& controller, const CompositeOperation& co,
	const LTS<std::string, std::string> topology, std::string& topology_state, std::vector<std::vector<std::string>>& parts) {
		for (const auto& tuple : co.sequential) {
			const auto& [op, input, output] = tuple;
			PCS_INFO("Handling operation: {} with input parts {} and output parts {}", op.name(), input, output);

			for (const auto& t : topology[topology_state].transitions_) {
				if (op.name() == t.first) { // Direct path
					size_t current_resource = CurrentResource(topology_state, t.second);
					if (!input.empty()) { // Check the input parts are present at the resource
						for (const auto& required_part : input) {
							if (std::find(parts[current_resource].begin(), parts[current_resource].end(), required_part) == parts[current_resource].end()) {
								// ... Remove part ... from parts[n][] as the operation has consumed it
							} else {
								PCS_WARN("Recipe is unreliasable due to missing part {}", required_part);
							}
						}
					}
					PCS_INFO("[Controller] Adding transition {} to {}. From resource {}", t.first, t.second, current_resource);
					controller.AddTransition(topology_state, t.first, t.second);
					topology_state = t.second;
					for (const auto& s : output) {
						parts[current_resource].push_back(s);
					}
				} else {  // Transfer path
					
				}
			}

		}
		return true;

	}


	// Recursively process each recipe state until there is none left, also flowing down the correct path based on the guard outcome
	// If the recipe cannot be realised at any point, false will be returned
	bool ProcessRecipe(LTS<std::string, std::string>& controller, const Machine& machine, const Recipe& recipe,
	const std::string& current_recipe_state, std::string& topology_state, std::vector<std::vector<std::string>>& parts) {
		if (recipe.lts()[current_recipe_state].transitions_.empty()) {
			return true; // No further recipe states to process
		}

		const LTS<std::string, std::string>& topology = machine.topology();
		const std::vector<LTS<std::string, std::string>>& resources = machine.resources();
		const CompositeOperation& co = recipe.lts()[current_recipe_state].transitions_[0].first; // assume 1st branch as guard passing pathway
		PCS_INFO("Processing recipe transition to: {} (guard check passed)", recipe.lts()[current_recipe_state].transitions_[0].second);

		bool handle = HandleComposite(controller, co, machine.topology(), topology_state, parts);
		if (handle == false) {
			return false;
		}
		
		const std::string& next_recipe_state = recipe.lts()[current_recipe_state].transitions_[0].second; // assume 1st branch as guard passing pathway
		ProcessRecipe(controller, machine, recipe, next_recipe_state, topology_state, parts);
	}

} // namespace end

 /*
	 @Todo: should have a controller generation class
	 @Todo: use LTS template parameters, i.e. vec<std::string> instead of std::string only
	 @Todo: separate logic for handling each operation
	 @Todo: guard operations, handle failure case
 */
