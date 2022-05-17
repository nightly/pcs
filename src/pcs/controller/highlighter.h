#pragma once

#include "pcs/lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/controller/controller.h"

#include <filesystem>

namespace pcs {

	void HighlightTopology(const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& topology,
		const LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>& controller, const std::filesystem::path& out_path);

}