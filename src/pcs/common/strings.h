#pragma once

#include <string>
#include <span>
#include <vector>

namespace pcs {

	std::string VectorToString(const std::vector<std::string>& vector);
	void StringToVector(const std::string& str, std::vector<std::string>& vec, char delimiter = ',');

}