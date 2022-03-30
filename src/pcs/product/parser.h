#pragma once

#include "nlohmann/json.hpp"

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {
	void ReadFromJsonFile(LabelledTransitionSystem<CompositeOperation>& lts, const std::filesystem::path& filepath);
	void ParseJson(LabelledTransitionSystem<CompositeOperation>& lts, const nlohmann::json& j);
}