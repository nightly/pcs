#pragma once

#include "pcs/lts/lts.h"

#include <string>
#include <filesystem>

namespace pcs::lts {

	void ReadFromFile(LabelledTransitionSystem& lts, const std::filesystem::path& filepath);
}