#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/lts/merge.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parser.h"
#include "pcs/lts/writers.h"


int main() {
	// Read LTS' & combine
	std::array<pcs::lts::LabelledTransitionSystem, 2> ltss;
	try {
		pcs::lts::ReadFromFile(ltss[0], "../../../../data/lts1.txt");
		pcs::lts::ReadFromFile(ltss[1], "../../../../data/lts1.txt"); // <- merges self, both lts1.txt
	} catch (std::ifstream::failure) {
		std::cerr << "Unable to read the file at specified path\n";
		return 1;
	}
	pcs::lts::LabelledTransitionSystem lts_combined = pcs::lts::Combine(ltss);

	// Console output
	for (size_t i = 0; i < ltss.size(); i++) {
		std::cout << "Labelled Transition System "<< (i+1) <<":\n" << ltss[i] << std::endl;
	}
	std::cout << "Combined LTS: \n" << lts_combined << '\n';

	// File output
	try {
		for (size_t i = 0; i < ltss.size(); i++) {
			std::string path = "../../../../exports/lts" + std::to_string(i + 1) + ".txt";
			pcs::lts::ExportToFile(ltss[i], path);
		}
		pcs::lts::ExportToFile(lts_combined, "../../../../exports/combined-lts.txt");
	} catch (std::ofstream::failure) {
		std::cerr << "Error writing to files or creating directory\n";
		return 1;
	}

	return 0;
}