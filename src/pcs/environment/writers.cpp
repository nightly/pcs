#include "pcs/environment/environment.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "pcs/lts/export.h"
#include "pcs/common/directory.h"

namespace pcs {

	/*
	 * @brief Exports machine topology & resources to a given directory.
	 * @param environment: the Environment to export
	 * @param directory: given directory to write the files to
	 * @exception: Propagates std::ofstream::failure
	 */
	void ExportEnvironment(const Environment& environment, const std::filesystem::path& directory) {
		std::ofstream stream;
		stream.exceptions(std::ofstream::badbit);
		CreateDirectoryForPath(directory);
		
		const std::vector<pcs::LTS<std::string, std::string>>& resources = environment.resources();
		for (size_t i = 0; i < environment.NumOfResources(); i++) {
			std::string r_path = directory.string() + '/' + "Resource" += std::to_string(i + 1) += ".txt";
			try {
				pcs::ExportToFile(resources[i], r_path);
			} catch (std::ofstream::failure& e) {
				throw;
			}
		}

		const auto& topology = environment.topology()->lts();
		std::string topology_path = directory.string() + '/' + "topology" + ".txt";
		try {
			pcs::ExportToFile(topology, topology_path);
		} catch (std::ofstream::failure& e) {
			throw;
		}
	}


}