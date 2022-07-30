#pragma once

#include "pcs/operation/parameters.h"
#include "pcs/operation/observable.h"

namespace pcs {

	bool Unify(const Parameters& operation_params, const Parameters& recipe_params, const pcs::Observable& operation);

}