#pragma once

#include <string>
#include <span>
#include <vector>

#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs {

	LabelledTransitionSystem<std::string> Combine(const std::span<LabelledTransitionSystem<std::string, std::string>>& ltss);
	void CombineRecursive(const std::span<LabelledTransitionSystem<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
		std::unordered_map<std::string, bool>& visited, LabelledTransitionSystem<std::string, std::string>& combined_lts);

	bool MatchingTransfer(const std::span<LabelledTransitionSystem<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
		size_t current_ltss_idx, const std::pair<std::string, std::string>& current_transition);
}