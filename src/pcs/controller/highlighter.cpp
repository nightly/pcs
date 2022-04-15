#include "pcs/controller/highlighter.h"

#include <filesystem>
#include <ostream>
#include <fstream>

#include "pcs/common/directory.h"
#include "pcs/common/log.h"

namespace pcs {

	/*
	 * @brief Returns a highlighted topology showing the path the controller took
	 */
	void HighlightTopology(const LTS<std::string, std::string>& topology,
	const LTS<std::string, std::string>& controller, const std::filesystem::path& out_path) {
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

			std::string target_state = controller.initial_state();
			std::pair<std::string, std::string> target_transition;
			if (!controller[target_state].transitions_.empty()) {
				target_transition = controller.states().at(target_state).transitions_[0];
			}

			for (const auto& pair : topology.states()) {
				auto& state = pair.second;
				if (state.IsEmpty()) {
					os << "	" << "\"" << state.name() << "\"" << "\n";
				}
				for (const auto& t : state.transitions_) {
					if ((target_state == state.name()) && (target_transition == t)) {
						os << "	" << "\"" << state.name() << "\"" << " -> " << "\"" << t.second << "\"" << " [color=\"blue\" label = " << "\"";
						os << t.first << "\"];\n";
						os << "	" << "\"" << state.name() << "\"" << " [shape=circle, style=filled, fillcolor=dodgerblue3]" << "\n";
						target_state = t.second;
						if (!controller[target_state].transitions_.empty()) {
							target_transition = controller.states().at(target_state).transitions_[0];
						}
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
	@Cleanup: should probably use a callback when dealing with a particular transition & particular state
*/
