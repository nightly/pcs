#include "pcs/controller/solvers/parse_costs.h"

#include <vector>
#include <filesystem>
#include <string>
#include <stdexcept>

#include "pcs/common/log.h"

namespace pcs {

	/**
	 * @exception Propagates std::ifstream::failure 
	 */
	std::vector<double> ParseCosts(const std::filesystem::path& path, size_t num_resources) {
		std::vector<double> ret;
		ret.reserve(num_resources);
		std::string line;

		try {
			std::ifstream stream(path);
			if (!stream.good()) {
				PCS_CRITICAL("Unable to read file for costs objective at path = {}", path.string());
				throw std::ifstream::failure("Unable to open costs file");
			}
			stream.exceptions(std::ifstream::badbit);
			while (std::getline(stream, line)) {
				if (line == "") {
					continue;
				}

				double d = stod(line);
				ret.emplace_back(d);
			}
		} catch (std::ifstream::failure& e) {
			throw;
		}

		return ret;
	}

}