#include <iostream>
#include <fstream>
#include <array>


#include "pcs/lts/lts.h"
#include "pcs/lts/merge.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parser.h"


int main() {
	std::array<pcs::lts::LabelledTransitionSystem, 2> ltss;
	try {
		pcs::lts::ReadFromFile(ltss[0], "../../../../data/lts1.txt");
		pcs::lts::ReadFromFile(ltss[1], "../../../../data/lts1.txt"); // <- merges self, both lts1.txt
	} catch (std::ifstream::failure) {
		std::cerr << "Unable to read the file at specified path" << std::endl;
		return 1;
	}

	std::cout << "Labelled Transition System 1: \n" << ltss[0] << std::endl;
	std::cout << "Labelled Transition System 2: \n" << ltss[1] << std::endl;
	pcs::lts::LabelledTransitionSystem lts_combined = pcs::lts::Combine(ltss);
	std::cout << "Combined LTS: \n" << lts_combined << std::endl;

	return 0;
}