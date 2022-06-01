#include "pcs/controller/highlighter.h"

#include <filesystem>
#include <ostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "pcs/common/directory.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"


namespace pcs {

	using ControllerState = std::vector<std::string>;
	using ControllerTransition = std::vector<std::string>;

	using TopologyState = std::vector<std::string>;
	using TopologyTransition = std::pair<size_t, std::string>;

	using SVecHash = boost::hash<std::vector<std::string>>;
	using TargetMapT = std::unordered_map<ControllerState, std::unordered_set<std::string>, SVecHash>;

	/*
	 * @brief Builds the target map, where KeyT = a target state, and ValueT = set of target transitions.
	 * 
	 * target_map: KeyT = Controller/Topology State and UnorderedSet values are the individual transitions
	 */
	 TargetMapT Highlighter::BuildTargetMap(const LTS<ControllerState, ControllerTransition, SVecHash>& controller) {
		 TargetMapT target_map;
		for (const auto& state : controller.states()) {
			target_map[state.first] = std::unordered_set<std::string>();
			for (const auto& t : state.second.transitions()) {
				// in the form (_, load, _, _, _) whereas we just want load
				std::vector<std::string> vec = t.label();
				for (auto& s : vec) {
					if (s == "" || s == "-") {
						continue;
					}
					target_map[state.first].insert(s);
				}
			}
		}
		return target_map;
	}

	/*
	 * @brief Returns a highlighted topology showing the path the controller took
	 */
	void Highlighter::HighlightTopology(const LTS<TopologyState, TopologyTransition, SVecHash>& topology,
		                  const LTS<ControllerState, ControllerTransition, SVecHash>& controller, const std::filesystem::path& out_path) {
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
					os << "	" << "\"" << pair.first << "\"" << "\n";
				}
				for (const auto& t : state.transitions_) {
					if ((target_map.contains(pair.first)) && (target_map[pair.first].contains(t.label().second))) {
						os << "	" << "\"" << pair.first << "\"" << " -> " << "\"" << t.to() << "\"" << " [color=\"royalblue4\" penwidth=2.25 label = " << "\"";
						os << t.label() << "\"];\n";
						os << "	" << "\"" << pair.first << "\"" << " [shape=circle, style=filled, fillcolor=dodgerblue2]" << "\n";
					} else {
						os << "	" << "\"" << pair.first << "\"" << " -> " << "\"" << t.to() << "\"" << " [label = " << "\"";
						os << t.label() << "\"];\n";
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