#pragma once

#include "pcs/lts/lts.h"

#include <string>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "pcs/operation/operation.h"

namespace pcs {

	void ReadFromFile(LTS<std::string, IOperation*>& lts, const std::filesystem::path& filepath);
	void ReadFromFile(LTS<std::vector<std::string>, IOperation&>& lts, const std::filesystem::path& filepath);

	void ReadFromJsonFile(LTS<std::string, IOperation*>& lts, const std::filesystem::path& filepath);
	void ParseJson(LTS<std::string, IOperation*>& lts, const nlohmann::json& j);
}