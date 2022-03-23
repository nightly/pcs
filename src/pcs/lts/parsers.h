#pragma once

#include "pcs/lts/lts.h"

#include <string>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace pcs::lts {

	void ReadFromFile(LabelledTransitionSystem& lts, const std::filesystem::path& filepath);
	void ReadFromJsonFile(LabelledTransitionSystem& lts, const std::filesystem::path& filepath);
	void ParseJson(LabelledTransitionSystem& lts, const nlohmann::json& j);
}