#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace pcs {

	template <typename StateT>
	StateT ParseStateString(const std::string& str);

}