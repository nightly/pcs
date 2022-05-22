#pragma once

#include "pcs/lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/controller/controller.h"

#include <filesystem>

namespace pcs {

	class Highlighter {
	public:
		using SVecHash = boost::hash<std::vector<std::string>>;
		using ControllerState = std::vector<std::string>;
		using ControllerTransition = std::vector<std::string>;

		using TopologyState = std::vector<std::string>;
		using TopologyTransition = std::pair<size_t, std::string>;

		using TargetMapT = std::unordered_map<ControllerState, std::unordered_set<std::string>, SVecHash>;
	public:
		static void HighlightTopology(const LTS<TopologyState, TopologyTransition, SVecHash>& topology,
			const LTS<ControllerState, ControllerTransition, SVecHash>& controller, const std::filesystem::path& out_path);
	private: 
		static TargetMapT BuildTargetMap(const LTS<ControllerState, ControllerTransition, SVecHash>& controller);
	};

}