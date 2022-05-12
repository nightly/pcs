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

	/*
	 * @brief Builds the target map, where KeyT = a target state, and ValueT = set of target transitions. 
	 * target_map = state_name to desired transitions
	 */
	static void BuildTargetMap(const LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>& controller, 
		                      std::unordered_map<std::vector<std::string>, std::unordered_set<std::string>, boost::hash<std::vector<std::string>>>& target_map) {
		for (const auto& state : controller.states()) {
			target_map[state.first] = std::unordered_set<std::string>();
			for (const auto& t : state.second.transitions()) {
				// in the form (_, load, _, _, _) whereas we just want load
				std::vector<std::string> vec = StringToVector(t.first);
				for (auto& s : vec) {
					if (s == "" || s == "-") {
						continue;
					}
					target_map[state.first].insert(s);
				}
			}
		}

	}

	/*
	 * @brief Returns a highlighted topology showing the path the controller took
	 */
	void HighlightTopology(const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& topology,
						  const LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>& controller, const std::filesystem::path& out_path) {
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

			std::unordered_map<std::vector<std::string>, std::unordered_set<std::string>, boost::hash<std::vector<std::string>>> target_map;
			BuildTargetMap(controller, target_map);


			for (const auto& pair : topology.states()) {
				auto& state = pair.second;
				if (state.IsEmpty()) {
					os << "	" << "\"" << state.name() << "\"" << "\n";
				}
				for (const auto& t: state.transitions_) {
					// @Cleanup: LTS types

					if ((target_map.count(state.name()) == 1) && (target_map[state.name()].contains(t.first.second))) {
						os << "	" << "\"" << state.name() << "\"" << " -> " << "\"" << t.second << "\"" << " [color=\"royalblue4\" penwidth=2.25 label = " << "\"";
						os << t.first << "\"];\n";
						os << "	" << "\"" << state.name() << "\"" << " [shape=circle, style=filled, fillcolor=dodgerblue2]" << "\n";
					} else {
						os << "	" << "\"" << state.name() << "\"" << " -> " << "\"" << t.second << "\"" << " [label = " << "\"";
						os << t.first << "\"];\n";
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
	@Cleanup: should probably use a functor as this duplicates some export code, but it's just for GraphViz visualisation
*/
