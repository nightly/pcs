#include "pcs/system/system.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "pcs/lts/export.h"
#include "pcs/common/directory.h"

namespace pcs {

	/*
	 * @brief Exports machine topology & resources to a given directory.
	 * @param machine: the Machine to export
	 * @param directory: given directory to write the files to
	 * @exception: Propagates std::ofstream::failure
	 */
	void ExportMachine(const System& system, const std::filesystem::path& directory) {
		std::ofstream stream;
		stream.exceptions(std::ofstream::badbit);
		CreateDirectoryForPath(directory);
		
		const std::vector<pcs::LTS<std::string, std::string>>& resources = system.resources();
		for (size_t i = 0; i < system.NumOfResources(); i++) {
			std::string r_path = directory.string() + '/' + "Resource" += std::to_string(i + 1) += ".txt";
			try {
				pcs::ExportToFile(resources[i], r_path);
			} catch (std::ofstream::failure& e) {
				throw;
			}
		}

		const pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& topology = system.topology();
		std::string topology_path = directory.string() + '/' + "topology" + ".txt";
		try {
			pcs::ExportToFile(topology, topology_path);
		} catch (std::ofstream::failure& e) {
			throw;
		}
	}


}