#include "pcs/controller/unify.h"

#include "pcs/operation/parameter.h"
#include "pcs/operation/observable.h"
#include "pcs/common/log.h"

namespace pcs {

	/*
	 * @brief Matches parameters between a recipe and a resource. Assumes the same order.  
	 * @param operation: only present for logging/additional information
	 */
	bool Unify(const std::vector<Parameter>& resource_params, const std::vector<Parameter>& recipe_params,  const pcs::Observable& operation) {
		if (recipe_params.size() != resource_params.size()) {
			PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
				"Mismatch between number of recipe parameters ({}) and resource parameters ({}) for operation {}", recipe_params.size(), resource_params.size(),
				operation.name()));
			return false;
		}

		for (size_t i = 0; i < recipe_params.size(); ++i) {

			if (recipe_params[i].name() != resource_params[i].name()) {
				PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
					"Mismatch between name of recipe parameter and resource parameter for operation {}", recipe_params[i].name(), resource_params[i].name(),
					operation.name()));
				return false;
			}

			if (resource_params[i].value() != "") {
				if (resource_params[i].value() != recipe_params[i].value()) {
					PCS_INFO(fmt::format(fmt::fg(fmt::color::hot_pink),
						"Resource parameter is non-constant {} which clashes with the recipe parameter of {} for operation {}", resource_params[i].value(), recipe_params[i].value(),
						operation.name()));
					return false;
				}
			}

		}
		return true;
	}

}