#include "pcs/lts/writers.h"

#include <string>
#include <ostream>
#include <filesystem>

#include "pcs/lts/lts.h"

namespace pcs::lts {

	/*
	 * @brief LTS output operator overload
	 */
	std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem& lts) {
		if (lts.initial_state_.empty() && lts.states_.empty()) {
			os << "Empty Labelled Transition System\n";
			return os;
		}
		os << "Initial state: " << lts.initial_state_ << '\n';
		for (const auto& pair : lts.states_) {
			os << pair.second;
		}
		return os;
	}

	/*
	 * @brief LTS file output operator overload 
	 */
	std::ofstream& operator<<(std::ofstream& os, const LabelledTransitionSystem& lts) {
		os << "digraph finite_state_machine {\n";
		os << "	fontname=\"Helvetica, Arial, sans - serif\"\n";
		os << "	node [fontname=\"Helvetica, Arial, sans - serif\"]\n";
		os << "	edge [fontname=\"Helvetica, Arial, sans - serif\"]\n";
		os << "	rankdir=LR;\n";
		os << "	node [shape = doublecircle];\n";
		os << "	" << "\"" << lts.initial_state_ << "\"" << ";\n";
		os << "	node [shape = circle];\n";
		for (const auto& pair : lts.states_) {
			os << pair.second;
		}
		os << "}";
		return os;
	}

	/*
	 * @brief State output operator overload
	 */
	std::ostream& operator<<(std::ostream& os, const State& state) {
		os << "State name: " << state.GetName() << '\n';
		if (state.transitions_.empty()) {
			os << "  With 0 transitions" << '\n';
			return os;
		}
		os << "  Transitions: " << '\n';
		for (const auto& pair : state.transitions_) {
			os << "    Label: " << pair.first << " " << "End State: " << pair.second << '\n';
		}
		return os;
	}

	/*
	 * @brief State file output operator overload 
	 */
	std::ofstream& operator<<(std::ofstream& os, const State& state) {
		if (state.IsEmpty()) {
			os << "	" << "\"" << state.name_ << "\"" << "\n";
		}
		for (const auto& t : state.transitions_) {
			os << "	" << "\"" << state.name_ << "\"" << " -> " << "\"" << t.second << "\"" 
				<< " [label = " << "\"" << t.first << "\"];\n";
		}
		return os;
	}

	/*
	 * @brief Exports a given LTS to the provided path, which can be visualised with GraphViz
	 * @param lts: labelled transition system in question to write to file
	 * @param path: the filepath to export to - if the given directory doesn't exist, it will be created
	 * @exception Propagates std::ofstream::failure
	 */
	void ExportToFile(const lts::LabelledTransitionSystem& lts, const std::filesystem::path& path) {
		std::ofstream stream;
		stream.exceptions(std::ofstream::badbit);
		if (!std::filesystem::is_directory(path.parent_path()) && path.has_parent_path()) {
			std::filesystem::create_directory(path.parent_path());
		}
		try {
			stream.open(path, std::ios::out | std::ios::trunc);
			stream << lts;
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}
}