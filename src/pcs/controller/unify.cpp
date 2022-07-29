#include "pcs/controller/unify.h"

#include "pcs/operation/parameter.h"
#include "pcs/operation/observable.h"
#include "pcs/common/log.h"

namespace pcs {

	/*
	 * @brief Matches parameters between a recipe and a resource. Assumes the same order.  
	 * @param operation: only present for logging/additional information
	 * @return Whether or not unification is successful and also updates resource_params for that operation.
	 */
	bool Unify(const std::vector<Parameter>& operation_params, const std::vector<Parameter>& recipe_params,  const pcs::Observable& operation) {
		if (recipe_params.size() != operation_params.size()) {
			PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
				"Mismatch between number of recipe parameters ({}) and resource parameters ({}) for operation {}",
				recipe_params.size(), operation_params.size(),
				operation.name()));
			return false;
		}

		for (size_t i = 0; i < recipe_params.size(); ++i) {

			if (recipe_params[i].name() != operation_params[i].name()) { // Ordered assumption
				PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
					"Mismatch between name of recipe parameter and resource parameter for operation {}'s parameter {}",
					recipe_params[i].name(), operation_params[i].name()));
				return false;
			}

			if (operation_params[i].value() == "") { 
				if (recipe_params[i].value() != "") {
					// Assign operation from recipe
				} else { // Both non-assigned
					PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
						"Operation parameter is non-assigned variable {} and the recipe parameter is non-assigned variable {}. No assignment possible for operation {}'s parameter {}",
						operation_params[i].value(), recipe_params[i].value(),
						operation.name(), operation_params[i].name()));
					return false;
				}
			} else {
				if (recipe_params[i].value() == "") {
					// Operation is assigned, recipe isn't - use the value from recipe.
				} else if (operation_params[i].value() != recipe_params[i].value()) {
					PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
						"Operation parameter is non-constant {} which clashes with the recipe parameter of {} for operation {}'s parameter {}",
						operation_params[i].value(), recipe_params[i].value(),
						operation.name(), operation_params[i].name()));
					return false;
				}
			}

		}
		return true;
	}

}