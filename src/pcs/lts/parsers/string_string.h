#pragma once

#include "pcs/lts/lts.h"

#include <string>
#include <vector>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace pcs {

	void ReadFromFile(LTS<std::string, std::string>& lts, const std::filesystem::path& filepath);
	void ReadFromFile(LTS<std::vector<std::string>, std::string>& lts, const std::filesystem::path& filepath);

	void ReadFromJsonFile(LTS<std::string, std::string>& lts, const std::filesystem::path& filepath);
	void ParseJson(LTS<std::string, std::string>& lts, const nlohmann::json& j);
}