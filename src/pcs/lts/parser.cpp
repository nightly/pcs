#include "pcs/lts/parser.h"

#include "pcs/lts/lts.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace pcs::lts {

	/*
	 * @brief ReadFromFile will parse an input file into the corresponding LTS class.
	 * @param lts: Labelled Transition System to work on
	 * @param filepath: path to the file containing a LTS, examples contained within the data folder.
	 * @exception Propagates std::ifstream::failure
	 */
	void ReadFromFile(LabelledTransitionSystem& lts, const std::string& filepath) {
		std::string line;
		bool first_line = true;
		try {
			std::ifstream stream(filepath);
			stream.exceptions(std::ifstream::badbit);
			while (std::getline(stream, line)) {
				if (first_line) {
					lts.SetInitialState(line);
					first_line = false;
					continue;
				}
				std::string start_state, label, end_state;
				std::istringstream ss(line);
				std::getline(ss, start_state, ' ');
				std::getline(ss, label, ' ');
				std::getline(ss, end_state);
				lts.AddSegment(start_state, label, end_state);
			}
		} catch (std::ifstream::failure e) {
			throw;
		}
	}
}
