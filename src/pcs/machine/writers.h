#pragma once

#include "pcs/machine/machine.h"
#include <filesystem>

namespace pcs {

	void ExportMachine(const Machine& machine, const std::filesystem::path& directory);

}