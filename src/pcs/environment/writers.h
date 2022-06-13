#pragma once

#include "pcs/environment/environment.h"
#include <filesystem>

namespace pcs {

	void ExportEnvironment(const Environment& machine, const std::filesystem::path& directory);

}