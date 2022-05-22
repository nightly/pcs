#pragma once

#include <string>
#include <span>
#include <vector>
#include <string_view>

namespace pcs {

	std::string VectorToString(const std::vector<std::string>& vector);
	std::vector<std::string> StringToVector(const std::string& str, char delimiter = ',');

}