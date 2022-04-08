#pragma once

#include "pcs/lts/lts.h"

#include <string>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace pcs {

	void ReadFromFile(LabelledTransitionSystem<std::string, std::string>& lts, const std::filesystem::path& filepath);
	void ReadFromJsonFile(LabelledTransitionSystem<std::string, std::string>& lts, const std::filesystem::path& filepath);
	void ParseJson(LabelledTransitionSystem<std::string, std::string>& lts, const nlohmann::json& j);
}