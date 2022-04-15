#pragma once

#include "nlohmann/json.hpp"

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {
	void ReadFromJsonFile(LTS<std::string, CompositeOperation>& lts, const std::filesystem::path& filepath);
	void ParseJson(LTS<std::string, CompositeOperation>& lts, const nlohmann::json& j);
}