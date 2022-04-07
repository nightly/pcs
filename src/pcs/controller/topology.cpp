#include "pcs/controller/topology.h"

#include <string>
#include <vector>
#include <span>
#include <set>
#include <utility>

#include <iostream>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/common/strings.h"

namespace pcs {

	/*
	 * @brief Combines multiple Labelled Transition Systems into one.
	 * @param ltss: lts' to combine
	 * @returns the combined LTS
	 */
	LabelledTransitionSystem<std::string> Combine(const std::span<LabelledTransitionSystem<std::string>>& ltss) {
		LabelledTransitionSystem combined_lts;
		std::vector<std::string> states_vec;
		for (const auto& lts : ltss) {
			// Populate with initial states
			states_vec.emplace_back(lts.GetInitialState());
		}
		std::string initial_key = VectorToString(states_vec);
		combined_lts.SetInitialState(initial_key, true);
		std::unordered_map<std::string, bool> visited;
		CombineRecursive(ltss, states_vec, visited, combined_lts);
		return combined_lts;
	}

	/*
	 * @brief CombineRecursive will recursively apply all transititions to generate a topology
	 * All possible transitions will be considered from each given state (_, _, _, _)
	 */
	void CombineRecursive(const std::span<LabelledTransitionSystem<std::string>>& ltss, std::vector<std::string>& states_vec,
	std::unordered_map<std::string, bool>& visited, LabelledTransitionSystem<std::string>& combined_lts) {
		std::string states_str = VectorToString(states_vec);
		if (visited[states_str] == true) {
			return;
		}

		visited[states_str] = true;
		for (size_t i = 0; i < ltss.size(); ++i) {
			for (const auto& t : ltss[i].states_.at(states_vec[i]).transitions_) {
				if (t.first.find("in:") != std::string::npos || t.first.find("out:") != std::string::npos) {
					bool found = MatchingTransfer(ltss, states_vec, i, t);
					if (!found) {
						continue;
					}
				}
				std::vector<std::string> next_states = states_vec;
				next_states[i] = t.second;
				combined_lts.AddTransition(states_str, t.first, VectorToString(next_states));
				CombineRecursive(ltss, next_states, visited, combined_lts);
			}
		}
	}

	/*
	 * @brief When coming across an "in:X" or "out:X" transition, a corresponding transfer transition is required
	 * as a transition in one of the states of the other resources
	 */
	bool MatchingTransfer(const std::span<LabelledTransitionSystem<std::string>>& ltss, std::vector<std::string>& states_vec,
	size_t current_ltss_idx, const std::pair<std::string, std::string>& current_transition) {
		bool is_in = current_transition.first.find("in:") != std::string::npos ? true : false; // In vs Out
		size_t n = is_in ? stoull(current_transition.first.substr(3)) : stoull(current_transition.first.substr(4));
		std::string match_string; 
		is_in ? match_string = "out:" : match_string = "in:";
		match_string += std::to_string(n);
		for (size_t i = 0; i < ltss.size(); ++i) {
			if (i == current_ltss_idx) {
				continue;
			}
			for (const auto& t : ltss[i].states_.at(states_vec[i]).transitions_) {
				if (t.first.find(match_string) != std::string::npos) {
					return true;
				}
			}
		}
		return false;
	}

	/*
	 * @brief RemoveResources facilitates the removal of resources from an existing topology without recomputing entirely
	 * @param topology: the pre-existing/current LTS topology
	 * @param resources: the resource to be removed relative to which position it is in the topology (_,x,_), x = 1,
	 *		  which coincides with the order of ltss' specified in the original span container of combine()
	 */
	void RemoveResources(LabelledTransitionSystem<>& topology, std::initializer_list<size_t> resources) {

	}

	/*
	 * @brief AddResources allows the addition of new resources to be added to an existing topology without recomputing entirely
	 * @param topology: the pre-existing/current LTS topology
	 * @param ltss: the new LTSS to add to be merged with the existing topology
	 */
	void AddResources(LabelledTransitionSystem<>& topology, const std::span<LabelledTransitionSystem<>>& ltss) {

	}

}