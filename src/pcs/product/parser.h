#pragma once

#include "nlohmann/json.hpp"

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {
	void ReadFromJsonFile(LabelledTransitionSystem<std::string, CompositeOperation>& lts, const std::filesystem::path& filepath);
	void ParseJson(LabelledTransitionSystem<std::string, CompositeOperation>& lts, const nlohmann::json& j);
}