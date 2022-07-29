#pragma once

#include "pcs/operation/parameter.h"
#include "pcs/operation/observable.h"

namespace pcs {

	bool Unify(const std::vector<Parameter>& operation_params, const std::vector<Parameter>& recipe_params, const pcs::Observable& operation);

}