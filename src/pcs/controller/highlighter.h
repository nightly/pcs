#pragma once

#include "pcs/lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/controller/controller.h"

#include <filesystem>

namespace pcs {

	void HighlightTopology(const LTS<std::string, std::string>& topology,  
		const LTS<std::string, std::string>& controller, const std::filesystem::path& out_path);

}