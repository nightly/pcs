#include "pcs/lts/parsers/state.h"

#include <vector>
#include <string>
#include <string_view>

#include "pcs/common/strings.h"

namespace pcs {

	template <>
	std::string ParseStateString(const std::string& str) {
		return str;
	}

	template <>
	std::vector<std::string> ParseStateString(const std::string& str) {
		return StringToVector(str);
	}

}