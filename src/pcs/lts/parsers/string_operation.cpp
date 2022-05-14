#include "pcs/lts/parsers/string_operation.h"

#include <string>
#include <filesystem>
#include <memory>

#include <nlohmann/json.hpp>

#include "pcs/operation/operation.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/nop.h"
#include "pcs/common/strings.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {


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
	void ReadFromFile(LTS<std::string, IOperation*>& lts, const std::filesystem::path& filepath) {
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

				// lts.AddTransition(start_state, *StringToOperation(label), end_state);
			}
		} catch (std::ifstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief ReadFromFile overload to specifically handle key type of std::vector<std::string> opposed to simply std::string 
	 */
	//void ReadFromFile(LTS<std::vector<std::string>, IOperation*>& lts, const std::filesystem::path& filepath) {
	//	std::string line;
	//	bool first_line = true;
	//	try {
	//		std::ifstream stream(filepath);
	//		stream.exceptions(std::ifstream::badbit);
	//		while (std::getline(stream, line)) {
	//			if (first_line) {
	//				lts.set_initial_state(StringToVector(line), true);
	//				first_line = false;
	//				continue;
	//			}
	//			std::string start_state, label, end_state;
	//			std::istringstream ss(line);
	//			std::getline(ss, start_state, ' ');
	//			std::getline(ss, label, ' ');
	//			std::getline(ss, end_state);

	//			// lts.AddTransition(StringToVector(start_state), StringToOperation(label), StringToVector(end_state));
	//		}
	//	} catch (std::ifstream::failure& e) {
	//		throw;
	//	}
	//}

}