#include "pcs/lts/parsers/state.h"

#include <vector>
#include <string>
#include <string_view>
#include <sstream>

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

	template <>
	std::pair<std::string, std::vector<std::string>> ParseStateString(const std::string& str) {
		constexpr char delimiter = '/';
		std::istringstream ss(str);
		std::string p1, p2_str;
		std::getline(ss, p1, delimiter);
		std::getline(ss, p2_str);
		std::vector<std::string> p2 = StringToVector(p2_str);
		return { p1, p2 };
	}

}