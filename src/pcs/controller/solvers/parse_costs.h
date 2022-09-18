#pragma once

#include <vector>
#include <filesystem>

namespace pcs {

	std::vector<double> ParseCosts(const std::filesystem::path& path, size_t num_resources);

}