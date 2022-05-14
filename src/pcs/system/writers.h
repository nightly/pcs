#pragma once

#include "pcs/system/system.h"
#include <filesystem>

namespace pcs {

	void ExportMachine(const System& machine, const std::filesystem::path& directory);

}