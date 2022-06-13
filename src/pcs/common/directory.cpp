#include "pcs/common/directory.h"

#include <filesystem>
#include <ostream>

namespace pcs {

	/*
	 * @brief Creates the parent directory/directories for a given path if it doesn't already exist
	 */
	void CreateDirectoryForPath(const std::filesystem::path& path) {
		if (!std::filesystem::is_directory(path.parent_path()) && path.has_parent_path()) {
			std::filesystem::create_directories(path.parent_path());
		}
	}

	void PrintCurrentDir(std::ostream& os) {
		os << std::filesystem::current_path() << std::endl;
	}

}