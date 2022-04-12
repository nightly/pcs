#include "pcs/lts/parsers/string_operation.h"

#include <string>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "pcs/operation/operation.h"
#include "pcs/operation/transfer.h"

namespace pcs {


	static IOperation StringToOperation(const std::string& label) {
		if (label.find("in:") != std::string::npos) {
			return TransferOperation(Transfer::in, stoull(label.substr(3)));
		} else if (label.find("out:") != std::string::npos) {
			return TransferOperation(Transfer::out, stoull(label.substr(4)));
		}
		return Operation(label);
	}

	/*
	 * @brief Reads a text file into an instance of LTS<Key = string, Transition = IOperation>
	 *
	 * The first line represents the initial state, and proceeding lines represent a
	 * transition, composed of the form:
	 *		StartState IOperation EndState
	 *
	 * @param lts: Labelled Transition System to parse into, of type LTS<string, IOperation>
	 * @param filepath: path to the file containing a LTS
	 * @exception Propagates std::ifstream::failure
	 */
	void ReadFromFile(LabelledTransitionSystem<std::string, IOperation>& lts, const std::filesystem::path& filepath) {
		std::string line;
		bool first_line = true;
		try {
			std::ifstream stream(filepath);
			stream.exceptions(std::ifstream::badbit);
			while (std::getline(stream, line)) {
				if (first_line) {
					lts.set_initial_state(line, true);
					first_line = false;
					continue;
				}
				std::string start_state, label, end_state;
				std::istringstream ss(line);
				std::getline(ss, start_state, ' ');
				std::getline(ss, label, ' ');
				std::getline(ss, end_state);

				lts.AddTransition(start_state, StringToOperation(label), end_state);
			}
		} catch (std::ifstream::failure& e) {
			throw;
		}
	}

}