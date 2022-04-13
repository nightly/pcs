#pragma once

#include "pcs/lts/lts.h"

#include <string>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "pcs/operation/operation.h"

namespace pcs {

	void ReadFromFile(LabelledTransitionSystem<std::string, IOperation*>& lts, const std::filesystem::path& filepath);
	void ReadFromFile(LabelledTransitionSystem<std::vector<std::string>, IOperation&>& lts, const std::filesystem::path& filepath);

	void ReadFromJsonFile(LabelledTransitionSystem<std::string, IOperation*>& lts, const std::filesystem::path& filepath);
	void ParseJson(LabelledTransitionSystem<std::string, IOperation*>& lts, const nlohmann::json& j);
}