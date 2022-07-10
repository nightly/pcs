#pragma once

#include <string>
#include <ostream>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/common/empty.h"

namespace pcs {


	// =========================
	// Generic structure output
	// =========================


	/**
	* @brief std::vector operator<< overload delimited by commas
	*/
	template <typename T>
	static std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
		for (std::size_t i = 0; i < vec.size(); ++i) {
			os << vec[i];
			if (i != vec.size() - 1) {
				os << ",";
			}
		}
		return os;
	}

	/**
	 * @brief std::pair operator<<
	 */
	template <typename T, typename U>
	static std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& pair) {
		os << pair.first;
		os << ": ";
		os << pair.second;
		return os;
	}

	// ==============================
	// LTS & State ostream & ofstream
	// ==============================

	template <typename KeyT, typename TransitionT>
	void PrintState(std::ofstream& os, const pcs::State<KeyT, TransitionT>& state, const KeyT& name) {
		if (state.IsEmpty()) {
			os << "	" << "\"" << name << "\"" << "\n";
			return;
		}
		for (const auto& t : state.transitions_) {
			os << "	" << "\"" << name << "\"" << " -> " << "\"" << t.to() << "\"" << " [label = " << "\"";
			os << t.label() << "\"];\n";
		}
	}

	template <typename KeyT, typename TransitionT, typename HashF>
	std::ofstream& operator<<(std::ofstream& os, const LTS<KeyT, TransitionT, HashF>& lts) {
		os << "digraph finite_state_machine {\n";
		os << "	fontname=\"Helvetica, Arial, sans - serif\"\n";
		os << "	node [fontname=\"Helvetica, Arial, sans - serif\"]\n";
		os << "	edge [fontname=\"Helvetica, Arial, sans - serif\"]\n";
		os << "	rankdir=LR;\n";
		os << "	node [shape = doublecircle];\n";
		os << "	" << "\"" << lts.initial_state() << "\"" << ";\n";
		os << "	node [shape = circle];\n";
		for (const auto& pair : lts.states()) {
			PrintState(os, pair.second, pair.first);
		}
		os << "}";
		return os;
	}

	template <typename KeyT, typename TransitionT>
	void PrintState(std::ostream& os, const pcs::State<KeyT, TransitionT>& state, const KeyT& name) {
		os << "State name: " << name << '\n';
		if (state.transitions_.empty()) {
			os << "  With 0 transitions" << '\n';
			return;
		}
		os << "  Transitions: " << '\n';
		for (const auto& pair : state.transitions_) {
			os << "    Label: " << pair.label() << " " << "End State: " << pair.to() << '\n';
		}
	}

	template <typename KeyT, typename TransitionT, typename HashF>
	std::ostream& operator<<(std::ostream& os, const LTS<KeyT, TransitionT, HashF>& lts) {
		if (lts.states_.empty()) {
			os << "Empty Labelled Transition System\n";
			return os;
		}
		os << "Initial state: " << lts.initial_state() << '\n';
		for (const auto& pair : lts.states()) {
			PrintState(os, pair.second, pair.first);
		}
		return os;
	}
}