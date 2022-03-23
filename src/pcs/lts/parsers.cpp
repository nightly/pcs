#include "pcs/lts/parsers.h"

#include "pcs/lts/lts.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

#include <nlohmann/json.hpp>

namespace pcs::lts {

	/*
	 * @brief ReadFromFile will parse an input file into an instance of the LTS class.
	 *
	 * The first line represents the initial state, and proceeding lines represent a
	 * transition, composed of the form:
	 *		StartState Action EndState
	 *
	 * @param lts: Labelled Transition System to parse into
	 * @param filepath: path to the file containing a LTS, examples contained within the data folder.
	 * @exception Propagates std::ifstream::failure
	 */
	void ReadFromFile(LabelledTransitionSystem& lts, const std::filesystem::path& filepath) {
		std::string line;
		bool first_line = true;
		try {
			std::ifstream stream(filepath);
			stream.exceptions(std::ifstream::badbit);
			while (std::getline(stream, line)) {
				if (first_line) {
					lts.SetInitialState(line, true);
					first_line = false;
					continue;
				}
				std::string start_state, label, end_state;
				std::istringstream ss(line);
				std::getline(ss, start_state, ' ');
				std::getline(ss, label, ' ');
				std::getline(ss, end_state);
				lts.AddTransition(start_state, label, end_state);
			}
		}
		catch (std::ifstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief ReadFromFile will parse a JSON input file into an instance of the LTS class.
	 *
	 * The expected form consists of: initialState as a string, and an array of transitions
	 * consisting of startState, label, and endState strings.
	 *
	 * @param lts: Labelled Transition System to parse into
	 * @param filepath: path to the file containing a LTS, examples contained within the data folder.
	 * @exception Propagates std::ifstream::failure
	 */
	void ReadFromJsonFile(LabelledTransitionSystem& lts, const std::filesystem::path& filepath) {
		nlohmann::json j;
		try {
			std::ifstream stream(filepath);
			stream >> j;
		}
		catch (std::ifstream::failure& e) {
			throw;
		}
		ParseJson(lts, j);
	}

	/*
	 * @brief ParseJson will read data into a LTS instance from a JSON object instance.
	 * @param lts: Labelled Transition System to parse into
	 * @param j: json object containing the "initialState" and "transitions" array of "startState", "label" & "endState"
	 */
	void ParseJson(LabelledTransitionSystem& lts, const nlohmann::json& j) {
		lts.SetInitialState(j["initialState"], true);
		for (const auto& t : j["transitions"]) {
			lts.AddTransition(t["startState"], t["label"], t["endState"], true);
		}
	}
}