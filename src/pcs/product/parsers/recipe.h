#pragma once

#include "nlohmann/json.hpp"

#include "lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {
	void ReadFromJsonFile(nightly::LTS<std::string, CompositeOperation>& lts, const std::filesystem::path& filepath);
	void ParseJson(nightly::LTS<std::string, CompositeOperation>& lts, const nlohmann::json& j);
}