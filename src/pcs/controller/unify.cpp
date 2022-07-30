#include "pcs/controller/unify.h"

#include "pcs/operation/parameters.h"
#include "pcs/operation/observable.h"
#include "pcs/common/log.h"

namespace pcs {

	/*
	 * @brief Matches parameters between a recipe and a resource. Assumes the same order.  
	 * @param operation: only present for logging/additional information
	 * @return Whether or not unification is successful and also updates resource_params for that operation.
	 */
	bool Unify(const Parameters& operation_params, const Parameters& recipe_params, const pcs::Observable& operation) {
		if (recipe_params.map().size() != operation_params.map().size()) {
			PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
				"Mismatch between number of recipe parameters ({}) and resource parameters ({}) for operation {}",
				recipe_params.map().size(), operation_params.map().size(),
				operation.name()));
			return false;
		}

		for (const auto& [rec_name, rec_value] : recipe_params.map()) {
			if (!operation_params.map().contains(rec_name)) {
				PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
					"Parameter variable {} with value {} is present in recipe, but not in the resource parameters for operation {}",
					rec_name, rec_value, operation.name()));
				return false;
			} 

			if (operation_params.map().at(rec_name) == "") {
				if (rec_value != "") {
					// Assign operation from recipe
				} else { // Both non-assigned
					PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
						"Operation parameter is non-assigned variable {} and the recipe parameter is non-assigned variable {}. No assignment possible for operation {}'s parameter {}",
						operation_params.map().at(rec_name), rec_value,
						operation.name(), rec_name));
					return false;
				}
			} else {
				if (rec_value == "") {
					// Assign operation from recipe
				} else if (operation_params.map().at(rec_name) != rec_value) {
					PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
						"Operation parameter is non-constant {} which clashes with the recipe parameter of {} for operation {}'s parameter {}",
						operation_params.map().at(rec_name), rec_value,
						operation.name(), rec_name));
					return false;
				}
			}

		}

		return true;
	}

}