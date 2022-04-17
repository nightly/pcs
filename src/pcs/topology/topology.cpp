#include "pcs/topology/topology.h"

#include <string>
#include <vector>
#include <span>
#include <set>
#include <utility>

#include <iostream>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/common/strings.h"
#include "pcs/operation/label.h"

namespace pcs {

	/*
	 * @brief Combines multiple Labelled Transition Systems into one.
	 * @param ltss: lts' to combine
	 * @returns the combined LTS
	 */
	LTS<std::string> Combine(const std::span<LTS<std::string, std::string>>& ltss) {
		LTS combined_lts;
		std::vector<std::string> states_vec;
		for (const auto& lts : ltss) {
			// Populate with initial states
			states_vec.emplace_back(lts.initial_state());
		}
		std::string initial_key = VectorToString(states_vec);
		combined_lts.set_initial_state(initial_key, true);
		std::unordered_map<std::string, bool> visited;
		CombineRecursive(ltss, states_vec, visited, combined_lts);
		return combined_lts;
	}

	/*
	 * @brief CombineRecursive will recursively apply all transititions to generate a topology
	 * All possible transitions will be considered from each given state (_, _, _, _)
	 */
	void CombineRecursive(const std::span<LTS<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
	std::unordered_map<std::string, bool>& visited, LTS<std::string>& combined_lts) {
		std::string states_str = VectorToString(states_vec);
		if (visited[states_str] == true) {
			return;
		}
		visited[states_str] = true;
		
		for (size_t i = 0; i < ltss.size(); ++i) {
			for (const auto& transition : ltss[i].states().at(states_vec[i]).transitions_) {
				if (transition.first.find("in:") != std::string::npos || transition.first.find("out:") != std::string::npos) {
				// @Bug: should consider the resulting state from both operations, not the singular one depicted here
					bool found = MatchingTransfer(ltss, states_vec, i, transition);
					if (!found) {
						continue;
					}
				}
				std::vector<std::string> next_states = states_vec;
				next_states[i] = transition.second;
				combined_lts.AddTransition(states_str, transition.first, VectorToString(next_states));
				CombineRecursive(ltss, next_states, visited, combined_lts);
			}
		}
	}

	/*
	 * @brief When coming across an "in:X" or "out:X" transition, a corresponding transfer transition is required
	 * as a transition in one of the states of the other resources
	 */
	bool MatchingTransfer(const std::span<LTS<std::string, std::string>>& ltss, std::vector<std::string>& states_vec,
	size_t current_ltss_idx, const std::pair<std::string, std::string>& current_transition) {
		TransferOperation transfer = *(StringToTransfer(current_transition.first));
		TransferOperation inverse = transfer.Inverse();

		for (size_t i = 0; i < ltss.size(); ++i) {
			if (i == current_ltss_idx) {
				continue;
			}
			for (const auto& t : ltss[i].states().at(states_vec[i]).transitions_) {
				if (t.first.find(inverse.name()) != std::string::npos) {
					return true;
				}
			}
		}
		return false;
	}

}