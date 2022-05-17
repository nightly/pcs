#include "merge.h"

#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>
#include <string>

#include "pcs/lts/lts.h"

#include "pcs/topology/complete.h"
#include "pcs/topology/topology.h"

#include "pcs/lts/state.h"
#include "pcs/lts/parsers/parsers.h"
#include "pcs/lts/export.h"
#include "pcs/lts/writers.h"
#include "pcs/common/log.h"

void MergeExample() {
	// Read LTS' & combine
	std::array<pcs::LTS<std::string>, 2> ltss;
	try {
		pcs::ReadFromFile(ltss[0], "../../data/lts/lts1.txt");
		pcs::ReadFromFile(ltss[1], "../../data/lts/lts2.txt");
	} catch (std::ifstream::failure) {
		std::cerr << "Unable to read the file at specified path\n";
		return std::exit(1);
	}

	pcs::CompleteTopology topology(ltss);
	pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> lts_combined = topology.lts();

	// Console output
	for (size_t i = 0; i < ltss.size(); i++) {
		std::cout << "Labelled Transition System " << (i + 1) << ":\n" << ltss[i] << std::endl;
	}
	std::cout << "Combined LTS: \n" << lts_combined << '\n';

	std::cout << "[Merge Example] Number of States = " << lts_combined.NumOfStates() << ". Number of Transitions = " << lts_combined.NumOfTransitions() << '\n';

	// File output
	try {
		for (size_t i = 0; i < ltss.size(); i++) {
			std::string path = "../../exports/merge/lts" + std::to_string(i + 1) + ".txt";
			pcs::ExportToFile(ltss[i], path);
		}
		pcs::ExportToFile(lts_combined, "../../exports/merge/combined-lts.txt");
	} catch (std::ofstream::failure) {
		std::cerr << "Error writing to files or creating directory\n";
		return std::exit(1);
	}
}