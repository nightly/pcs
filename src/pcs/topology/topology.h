#pragma once

#include <string>
#include <span>
#include <vector>
#include <optional>
#include <unordered_set>

#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs {

	LTS<std::string> Combine(const std::span<LTS<std::string, std::string>>& ltss);

	void CombineRecursive(const std::span<LTS<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
		                 std::unordered_set<std::string>& visited, LTS<std::string, std::string>& combined_lts);

	std::optional<std::vector<std::string>> MatchingTransfer(const std::span<LTS<std::string, std::string>>& ltss, const std::vector<std::string>& states_vec,
															size_t current_ltss_idx, const std::pair<std::string, std::string>& current_transition);

}