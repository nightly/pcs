#pragma once

#include <vector>
#include <span>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

#include <nlohmann/json.hpp>

#include "pcs/lts/lts.h"
#include "pcs/operation/operation.h"
#include "pcs/lts/parsers/state.h"
#include "pcs/lts/parsers/transition.h"
#include "pcs/common/strings.h"

namespace pcs {

	/**
	 * @brief Parses a text file into LTS (see `data/` folder for examples)
	 * First line of the file sets the initial state. Following lines add a transition in the form of:
	 *		StartState Action EndState
	 * @param lts: the Labelled Transition System to parse into
	 * @param path: path to text file
	 * @exception Propagates std::ifstream failure
	 */
	template <typename StateT, typename TransitionT, typename HashF>
	void ReadFromFile(LTS<StateT, TransitionT, HashF>& lts, const std::filesystem::path& filepath) {
		std::string line;
		bool first_line = true;
		try {
			std::ifstream stream(filepath);
			stream.exceptions(std::ifstream::badbit);
			while (std::getline(stream, line)) {
				if (first_line) {
					std::string initial_state_str = line;
					StateT initial_state = ParseStateString<StateT>(line);
					lts.set_initial_state(initial_state);
					first_line = false;
					continue;
				}

				std::string start_state_str, label_str, end_state_str;
				StateT start_state, end_state;
				TransitionT label;

				std::istringstream ss(line);
				std::getline(ss, start_state_str, ' ');
				std::getline(ss, label_str, ' ');
				std::getline(ss, end_state_str);

				start_state = ParseStateString<StateT>(Trim(start_state_str));
				end_state = ParseStateString<StateT>(Trim(end_state_str));
				label = ParseTransitionString<TransitionT>(Trim(label_str));

				lts.AddTransition(start_state, label, end_state);
			}
		} catch (std::ifstream::failure& e) {
			throw;
		}
	}

	template <typename StateT, typename TransitionT, typename HashF>
	void ParseJson(LTS<StateT, TransitionT, HashF>& lts, const nlohmann::json& j) {
		std::string initial_state_str = j["initialState"];
		StateT initial_state = ParseStateString<StateT>(initial_state_str);
		lts.set_initial_state(initial_state);

		for (const auto& t : j["transitions"]) {
			std::string start_state_str, label_str, end_state_str;
			StateT start_state, end_state;
			TransitionT label;

			start_state_str = t["startState"];
			label_str = t["label"];
			end_state_str = t["endState"];

			start_state = ParseStateString<StateT>(start_state_str);
			end_state = ParseStateString<StateT>(end_state_str);
			label = ParseTransitionString<TransitionT>(label_str);
		
			lts.AddTransition(start_state, label, end_state);
		}
	}

	/**
	 * @brief Parses a JSON file into a Labelled Transition System 
	 * The expected form consists of: initialState as a string, and an array of transitions
	 * consisting of startState, label, and endState strings.
	 */
	template <typename StateT, typename TransitionT, typename HashF>
	void ReadFromJsonFile(LTS<StateT, TransitionT, HashF>& lts, const std::filesystem::path& filepath) {
		nlohmann::json j;
		try {
			std::ifstream stream(filepath);
			stream >> j;
		} catch (std::ifstream::failure& e) {
			throw;
		}
		ParseJson(lts, j);
	}


}