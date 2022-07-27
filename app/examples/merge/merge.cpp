#include "merge.h"

#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>
#include <string>

#include "lts/lts.h"

#include "pcs/topology/complete.h"
#include "pcs/topology/topology.h"

#include "lts/state.h"
#include "lts/parsers/parsers.h"
#include "lts/export.h"
#include "lts/writers.h"
#include "pcs/common/log.h"

/*
 * @param number: the amount of LTS' to merge
 * @see: benchmarks/range/topology/complete/range.cpp for this with benchmarks using PAD R1
 */
void MergeExample(size_t number) {
	// Read LTS' & combine
	std::vector<nightly::LTS<std::string, pcs::ParameterizedOp>> ltss;
	ltss.reserve(number);
	for (size_t i = 1; i <= number; ++i) {
		try {
			nightly::LTS<std::string, pcs::ParameterizedOp> lts;
			if (i % 2 == 0) {
				nightly::ReadFromFile(lts, "../../data/lts/lts2.txt");
			} else {
				nightly::ReadFromFile(lts, "../../data/lts/lts1.txt");
			}
			ltss.emplace_back(lts);
		} catch (std::ifstream::failure) {
			std::cerr << "Unable to read the file at specified path\n";
			return std::exit(1);
		}
	}

	pcs::CompleteTopology topology(ltss);
	nightly::LTS<std::vector<std::string>, std::pair<size_t, pcs::ParameterizedOp>, boost::hash<std::vector<std::string>>> lts_combined = topology.lts();

	// Console output
	for (size_t i = 0; i < ltss.size(); i++) {
		std::cout << "Labelled Transition System " << (i + 1) << ":\n" << ltss[i] << std::endl;
	}
	std::cout << "Combined LTS: \n" << lts_combined << '\n';

	std::cout << "[Merge Example] Number of Resources = " << ltss.size() << '\n';
	std::cout << "[Merge Example: LTS-1] Number of States = " << ltss[0].NumOfStates() << ". Number of Transitions = "
		<< ltss[0].NumOfTransitions() << '\n';
	std::cout << "[Merge Example: LTS-2] Number of States = " << ltss[1].NumOfStates() << ". Number of Transitions = "
		<< ltss[1].NumOfTransitions() << '\n';
	std::cout << "[Merge Example: Topology] Number of States = " << lts_combined.NumOfStates() << ". Number of Transitions = " 
		<< lts_combined.NumOfTransitions() << '\n';

	// File output
	try {
		for (size_t i = 0; i < ltss.size(); i++) {
			std::string path = "../../exports/merge/lts" + std::to_string(i + 1) + ".gv";
			nightly::ExportToFile(ltss[i], path);
		}
		nightly::ExportToFile(lts_combined, "../../exports/merge/combined-lts.gv");
	} catch (std::ofstream::failure) {
		std::cerr << "Error writing to files or creating directory\n";
		return std::exit(1);
	}
}