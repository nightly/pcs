#pragma once

#include "lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/controller/controller.h"

#include <filesystem>

namespace pcs {

	class Highlighter {
	public:
		using ControllerState = std::pair<std::string, std::vector<std::string>>;
		using ControllerTransition = std::vector<std::string>;

		using TopologyState = std::vector<std::string>;
		using TopologyTransition = std::pair<size_t, ParameterizedOp>;

		using TargetMapT = std::unordered_map<TopologyState, std::unordered_set<std::string>, boost::hash<TopologyState>>;
	public:
		static void HighlightTopology(const nightly::LTS<TopologyState, TopologyTransition, boost::hash<TopologyState>> & topology,
			                          const nightly::LTS<ControllerState, ControllerTransition, boost::hash<ControllerState>>& controller, 
			                          const std::filesystem::path& out_path,
			                          const bool textless = false);
	private: 
		static TargetMapT BuildTargetMap(const nightly::LTS<ControllerState, ControllerTransition, boost::hash<ControllerState>>& controller);
	};

}