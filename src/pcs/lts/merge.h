#pragma once

#include <string>
#include <span>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs::lts {

	LabelledTransitionSystem Combine(const std::span<LabelledTransitionSystem>& ltss);
	void CombineRecursive(const std::span<LabelledTransitionSystem>& ltss, std::vector<std::string>& state_strings,
		LabelledTransitionSystem& combined_lts);
	std::string StateVectorToString(const std::span<std::string>& vec);
	class merge {
	public:
		std::string startstate = "s0";
		std::vector<std::pair<std::string, std::string>> topology;
	private:
		int size;
	public:
		merge();
		void createVisitedDictionary(std::vector<LabelledTransitionSystem> lts, std::map<std::string, bool>& isVisited);
		void merger(std::vector<LabelledTransitionSystem> lts);
		void combine(std::vector<LabelledTransitionSystem> lts, std::vector<std::string> currState, std::map<std::string, bool>& isVisited);

	};
}
