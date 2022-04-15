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

	ControllerGenerator::ControllerGenerator(const Machine& machine, const Recipe& recipe) 
		: machine_(machine), recipe_(recipe), topology_(machine.topology()) {}
	
	std::optional<const LTS<std::string, std::string>*> ControllerGenerator::Generate() {
		parts_.resize(machine_.NumOfResources());
		recipe_state_ = recipe_.lts().initial_state();
		topology_state_ = topology_.initial_state();
		controller_.set_initial_state(topology_state_, true);

		PCS_INFO("Controller initial state {}", controller_.initial_state());
		PCS_INFO("Recipe initial state: {}", recipe_.lts().initial_state());

		bool successful_generation = ProcessRecipe(recipe_state_);
		if (!successful_generation) {
			return {};
		}
		return &controller_;
	}

	bool ControllerGenerator::ProcessRecipe(const std::string& state) {
		if (recipe_.lts()[state].transitions_.empty()) {
			return true; // No further recipe states to process
		}

		const CompositeOperation& co = recipe_.lts()[recipe_state_].transitions_[0].first; // assume 1st branch as guard passing pathway
		PCS_INFO("Processing recipe transition to: {} (guard check passed)", recipe_.lts()[recipe_state_].transitions_[0].second);

		bool handle = HandleComposite(co);
		if (handle == false) {
			return false;
		}

		const std::string& next_recipe_state = recipe_.lts()[state].transitions_[0].second; // assume 1st branch as guard passing pathway
		ProcessRecipe(next_recipe_state);
	}


	bool ControllerGenerator::HandleComposite(const CompositeOperation& co) {
		for (const auto& tuple : co.sequential) {
			const auto& [op, input, output] = tuple;
			PCS_INFO("Handling operation: {} with input parts {} and output parts {}", op.name(), input, output);

			for (const auto& t : topology_[topology_state_].transitions_) {
				if (op.name() == t.first) { // Direct path
					size_t current_resource = CurrentResource(topology_state_, t.second);
					if (!input.empty()) { // Check the input parts are present at the resource
						for (const auto& required_part : input) {
							if (std::find(parts_[current_resource].begin(), parts_[current_resource].end(), required_part) == parts_[current_resource].end()) {
								// ... Remove part ... from parts[n][] as the operation has consumed it
							} else {
								PCS_WARN("Recipe is unreliasable due to missing part {}", required_part);
							}
						}
					}
					PCS_INFO("[Controller] Adding transition {} to {}. From resource {}", t.first, t.second, current_resource);
					controller_.AddTransition(topology_state_, t.first, t.second);
					topology_state_ = t.second;
					for (const auto& s : output) {
						parts_[current_resource].push_back(s);
					}
				} else {  // Transfer path
					
				}
			}

		}
		return true;

	}

	// Identifies current resource being reviewed based on old topology string state and new string state
	size_t ControllerGenerator::CurrentResource(const std::string& old_state, const std::string& new_state) {
		std::vector<std::string> old_ve = StringToVector(old_state);
		std::vector<std::string> new_ve = StringToVector(new_state);
		for (size_t i = 0; i < new_ve.size(); ++i) {
			if (old_ve[i] != new_ve[i])
				return i;
		}
	}

} // namespace end

 /*
	 @Todo: use LTS template parameters, i.e. vec<std::string> instead of std::string only
	 @Todo: separate logic for handling each operation
	 @Todo: guard operations, handle failure case
 */
