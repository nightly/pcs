#include "pcs/controller/highlighter.h"

#include <filesystem>
#include <ostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

#include "pcs/common/directory.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "lts/lts.h"
#include "lts/transition.h"

namespace pcs {

	using namespace nightly;

	using ControllerState = std::pair<std::string, std::vector<std::string>>;
	using ControllerTransition = std::vector<std::string>;

	using TopologyState = std::vector<std::string>;
	using TopologyTransition = std::pair<size_t, ParameterizedOp>;

	using TargetMapT = std::unordered_map<TopologyState, std::unordered_set<std::string>, boost::hash<TopologyState>>;

	/*
	 * @brief Builds the target map, where KeyT = a target state, and ValueT = set of target transitions.
	 * 
	 * target_map: KeyT = Topology State and values are an unordered set of transitions applicable to that state
	 */
	 TargetMapT Highlighter::BuildTargetMap(const nightly::LTS<ControllerState, ControllerTransition, 
		                                   boost::hash<ControllerState>>& controller) {
		 TargetMapT target_map;
		for (const auto& state : controller.states()) {
			target_map[state.first.second] = std::unordered_set<std::string>();
			for (const auto& t : state.second.transitions()) {
				// in the form (_, load, _, _, _) whereas we just want load
				std::vector<std::string> vec = t.label();
				for (auto& s : vec) {
					if (s == "" || s == "-") {
						continue;
					}
					target_map[state.first.second].insert(s);
				}
			}
		}
		return target_map;
	}

	/*
	 * @brief Returns a highlighted topology showing the path the controller took
	 */
	void Highlighter::HighlightTopology(const nightly::LTS<TopologyState, TopologyTransition, boost::hash<TopologyState>>& topology,
		                               const nightly::LTS<ControllerState, ControllerTransition, boost::hash<ControllerState>>& controller, 
		                               const std::filesystem::path& out_path) {
		std::ofstream os;
		os.exceptions(std::ofstream::badbit);
		CreateDirectoryForPath(out_path);

		try {
			os.open(out_path, std::ios::out | std::ios::trunc);
			os << "digraph finite_state_machine {\n";
			os << "	fontname=\"Helvetica, Arial, sans - serif\"\n";
			os << "	node [fontname=\"Helvetica, Arial, sans - serif\"]\n";
			os << "	edge [fontname=\"Helvetica, Arial, sans - serif\"]\n";
			os << "	rankdir=LR;\n";
			os << "	node [shape = doublecircle];\n";
			os << "	" << "\"" << topology.initial_state() << "\"" << ";\n";
			os << "	node [shape = circle];\n";

			TargetMapT target_map = BuildTargetMap(controller);

			for (const auto& pair : topology.states()) {
				auto& state = pair.second;
				if (state.IsEmpty()) {
					if (target_map.contains(pair.first)) {
						os << "	" << "\"" << pair.first << "\"" << " [shape=circle, style=filled, fillcolor=dodgerblue2]" << "\n";
					} else {
						os << "	" << "\"" << pair.first << "\"" << "\n";
					}
				}
				for (const auto& t : state.transitions_) {
					if ((target_map.contains(pair.first)) && (target_map[pair.first].contains(t.label().second.operation()))) {
						os << "	" << "\"" << pair.first << "\"" << " -> " << "\"" << t.to() << "\"" << " [color=\"royalblue4\" penwidth=2.25 label = " << "<";
						os << t.label() << ">];\n";
						os << "	" << "\"" << pair.first << "\"" << " [shape=circle, style=filled, fillcolor=dodgerblue2]" << "\n";
					} else {
						os << "	" << "\"" << pair.first << "\"" << " -> " << "\"" << t.to() << "\"" << " [label = " << "<";
						os << t.label() << ">];\n";
					}
				}
			}
			os << "}";
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}


}


/*
	@Note: `  c -> d [color="blue"]`

	@Cleanup: should probably use a functor because this duplicates output/styling code although unimportant 
	@Cleanup: should use ITopology
*/