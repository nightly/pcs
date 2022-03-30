#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/controller/topology.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parsers.h"
#include "pcs/lts/writers.h"

int main() {
	// Read LTS' & combine
	std::array<pcs::LabelledTransitionSystem<>, 4> ltss;
	try {
		pcs::ReadFromFile(ltss[0], "../../data/lts1.txt");
		pcs::ReadFromFile(ltss[1], "../../data/lts2.txt");
		pcs::ReadFromFile(ltss[2], "../../data/lts3.txt");
		pcs::ReadFromFile(ltss[3], "../../data/lts4.txt");
	} catch (std::ifstream::failure) {
		std::cerr << "Unable to read the file at specified path\n";
		return 1;
	}
	pcs::LabelledTransitionSystem lts_combined = pcs::Combine(ltss);

	// Console output
	for (size_t i = 0; i < ltss.size(); i++) {
		std::cout << "Labelled Transition System "<< (i+1) <<":\n" << ltss[i] << std::endl;
	}
	std::cout << "Combined LTS: \n" << lts_combined << '\n';

	// File output
	try {
		for (size_t i = 0; i < ltss.size(); i++) {
			std::string path = "../../exports/lts" + std::to_string(i + 1) + ".txt";
			pcs::ExportToFile(ltss[i], path);
		}
		pcs::ExportToFile(lts_combined, "../../exports/combined-lts.txt");
	} catch (std::ofstream::failure) {
		std::cerr << "Error writing to files or creating directory\n";
		return 1;
	}

	return 0;
}