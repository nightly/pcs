#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace pcs {

	template <typename TransitionT>
	TransitionT ParseTransitionString(const std::string& str);

}