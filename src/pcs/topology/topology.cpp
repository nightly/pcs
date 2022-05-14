#include "pcs/topology/topology.h"

#include <string>
#include <vector>
#include <span>
#include <set>
#include <utility>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/common/log.h"

namespace pcs {

	/*
	 * @brief Combines multiple Labelled Transition Systems into one.
	 * @param ltss: lts' to combine
	 * @returns the combined LTS of form KeyT = string (combined state), TransitionT = pair<size_t, string> (acting on resource, action)
	 */
	LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> Combine(const std::span<LTS<std::string, std::string>>& ltss) {
		LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> combined_lts;
		std::vector<std::string> initial_key;
		for (const auto& lts : ltss) {
			// Populate with initial states
			initial_key.emplace_back(lts.initial_state());
		}
		combined_lts.set_initial_state(initial_key, true);
		std::unordered_set<std::vector<std::string>, boost::hash<std::vector<std::string>>> visited;
		CombineRecursive(ltss, initial_key, visited, combined_lts);
		return combined_lts;
	}

	/*
	 * @brief CombineRecursive will recursively apply all transititions to generate a topology
	 * All possible transitions will be considered from each given state (_, _, _, _)
	 */
	void CombineRecursive(const std::span<LTS<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
		                  std::unordered_set<std::vector<std::string>, boost::hash<std::vector<std::string>>>& visited,
		                 LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& combined_lts) {
		if (visited.contains(states_vec) == true) {
			return;
		}
		visited.insert(states_vec);
		
		for (size_t i = 0; i < ltss.size(); ++i) {
			for (const auto& transition : ltss[i].states().at(states_vec[i]).transitions_) {
				if ((transition.first.find("in:") != std::string::npos) || (transition.first.find("out:") != std::string::npos)) {
					std::optional<std::vector<std::string>> transfer_state = MatchingTransfer(ltss, states_vec, i, transition);
					if (!transfer_state.has_value()) {
						continue;
					}
					combined_lts.AddTransition(states_vec, std::make_pair(i, transition.first), *transfer_state);
					CombineRecursive(ltss, *transfer_state, visited, combined_lts); 
				} else {
					std::vector<std::string> next_states = states_vec;
					next_states[i] = transition.second;
					combined_lts.AddTransition(states_vec, std::make_pair(i, transition.first), next_states);
					CombineRecursive(ltss, next_states, visited, combined_lts);
				}
			}
		}
	}

	/*
	 * @brief When coming across an "in:X" or "out:X" transition, a corresponding inverse is required.
	 * @return The end-state of applying the two transitions if found.
	 */
	std::optional<std::vector<std::string>> MatchingTransfer(const std::span<LTS<std::string, std::string>>& ltss, const std::vector<std::string>& states_vec,
		size_t current_ltss_idx, const std::pair<std::string, std::string>& current_transition) {
		TransferOperation transfer = *(StringToTransfer(current_transition.first));
		TransferOperation inverse = transfer.Inverse();

		for (size_t i = 0; i < ltss.size(); ++i) {
			if (i == current_ltss_idx) {
				continue;
			}
			for (const auto& t : ltss[i].states().at(states_vec[i]).transitions_) {
				if (t.first.find(inverse.name()) != std::string::npos) {
					std::vector<std::string> resulting_state = states_vec;
					resulting_state[current_ltss_idx] = current_transition.second;
					resulting_state[i] = t.second;
					return resulting_state;
				}
			}
		}
		return {};
	}

}