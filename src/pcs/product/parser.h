#pragma once

#include "nlohmann/json.hpp"

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs::product {
	void ReadFromJsonFile(lts::LabelledTransitionSystem<CompositeOperation>& lts, const std::filesystem::path& filepath);
	void ParseJson(lts::LabelledTransitionSystem<CompositeOperation>& lts, const nlohmann::json& j);
}