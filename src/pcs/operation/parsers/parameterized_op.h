#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <sstream>

#include "pcs/operation/parameterized_op.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/operation/parameters.h"

namespace pcs {

	pcs::Parameters StringToParameters(const std::string& str);
	ParameterizedOp StringToParameterizedOp(const std::string& str);
	
}
