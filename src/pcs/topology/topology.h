#pragma once

#include <string>
#include <span>
#include <vector>
#include <optional>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>


#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs {

	LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> Combine(const std::span<LTS<std::string, std::string>>& ltss);

	void CombineRecursive(const std::span<LTS<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
		                 std::unordered_set<std::vector<std::string>, boost::hash<std::vector<std::string>>>& visited, 
		                 LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& combined_lts);

	std::optional<std::vector<std::string>> MatchingTransfer(const std::span<LTS<std::string, std::string>>& ltss, const std::vector<std::string>& states_vec,
															size_t current_ltss_idx, const std::pair<std::string, std::string>& current_transition);

}