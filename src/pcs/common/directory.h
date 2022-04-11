#pragma once

#include <filesystem>
#include <iostream>
#include <ostream>

namespace pcs {

	void CreateDirectoryForPath(const std::filesystem::path& path);
	void PrintCurrentDir(std::ostream& os = std::cout);
}