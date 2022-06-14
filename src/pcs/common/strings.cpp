#include "pcs/common/strings.h"

#include <span>
#include <string>
#include <sstream>

namespace pcs {

	/*
	 * @brief Transforms a vector of strings into a singular state string (_,_,_,_)
	 */
	std::string VectorToString(const std::vector<std::string>& vec) {
		std::string ret;
		for (size_t i = 0; i < vec.size(); i++) {
			ret += vec[i];
			if (i != vec.size() - 1) {
				ret += ',';
			}
		}
		return ret;
	}

	/*
	 * @brief Transforms (_,_,_) into a vector 
	 * @param delimiter: default argument of ','
	 */
	std::vector<std::string> StringToVector(const std::string& str, char delimiter) {
		std::vector<std::string> vec;
		std::stringstream stream(str);
		std::string cur;
		while (std::getline(stream, cur, delimiter)) {
			vec.emplace_back(cur);
		}
		return vec;
	}

	/*
	 * @param delimiter: default = ',' 
	 */
	std::string USetToString(const std::unordered_set<std::string>& set, char delimiter) {
		std::string ret;
		for (auto it = set.begin(); it != set.end(); ++it) {
			if (it != set.begin()) {
				ret += delimiter;
			}
			ret += *it;
		}
		return ret;
	}

}