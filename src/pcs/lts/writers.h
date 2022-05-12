#pragma once

#include <string>
#include <ostream>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"

namespace pcs {


	// ================
	// Generic output
	// ================

	/*
	 * @brief std::pair operator<<
	 */
	template<typename T, typename U>
	std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& pair)
	{
		os << pair.first;
		os << ": ";
		os << pair.second;
		return os;
	}

	/*
	* @brief std::vector operator<< overload delimited by commas
	*/
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
	{
		for (std::size_t i = 0; i < vec.size(); ++i) {
			os << vec[i];
			if (i != vec.size() - 1) {
				os << ",";
			}
		}
		return os;
	}


	// ==============================
	// LTS & State ostream & ofstream
	// ==============================

	template <typename KeyT, typename TransitionT>
	std::ofstream& operator<<(std::ofstream& os, const internal::State<KeyT, TransitionT>& state) {
		if (state.IsEmpty()) {
			os << "	" << "\"" << state.name() << "\"" << "\n";
		}
		for (const auto& t : state.transitions_) {
			os << "	" << "\"" << state.name() << "\"" << " -> " << "\"" << t.second << "\"" << " [label = " << "\"";
			os << t.first << "\"];\n";
		}
		return os;
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
			os << pair.second;
		}
		os << "}";
		return os;
	}

	template <typename KeyT, typename TransitionT>
	std::ostream& operator<<(std::ostream& os, const internal::State<KeyT, TransitionT>& state) {
		os << "State name: " << state.name() << '\n';
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

	template <typename KeyT, typename TransitionT, typename HashF>
	std::ostream& operator<<(std::ostream& os, const LTS<KeyT, TransitionT, HashF>& lts) {
		if (lts.initial_state().empty() && lts.states_.empty()) {
			os << "Empty Labelled Transition System\n";
			return os;
		}
		os << "Initial state: " << lts.initial_state() << '\n';
		for (const auto& pair : lts.states()) {
			os << pair.second;
		}
		return os;
	}
}