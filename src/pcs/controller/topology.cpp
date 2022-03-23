#include "pcs/controller/topology.h"

#include <string>
#include <vector>
#include <span>
#include <set>
#include <utility>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"

namespace pcs::lts {

	/*
	 * @brief Combines multiple Labelled Transition Systems into one.
	 * @param ltss: lts' to combine
	 * @returns the combined LTS
	 */
	LabelledTransitionSystem Combine(const std::span<LabelledTransitionSystem>& ltss) {
		LabelledTransitionSystem combined_lts;
		std::vector<std::string> states_vec;
		for (const auto& lts : ltss) {
			// Populate with initial states
			states_vec.emplace_back(lts.GetInitialState());
		}
		std::string initial_key = StateVectorToString(states_vec);
		combined_lts.SetInitialState(initial_key, true);
		std::unordered_map<std::string, bool> visited;
		CombineRecursive(ltss, states_vec, visited, combined_lts);
		return combined_lts;
	}

	/*
	 * @brief CombineRecursive will recursively apply all transititions to generate a topology
	 * All possible transitions will be considered from each given state (_, _, _, _)
	 */
	void CombineRecursive(const std::span<LabelledTransitionSystem>& ltss, std::vector<std::string>& states_vec,
	std::unordered_map<std::string, bool>& visited, LabelledTransitionSystem& combined_lts) {
		std::string states_str = StateVectorToString(states_vec);
		if (visited[states_str] == true) {
			return;
		}
		visited[states_str] = true;
		for (size_t i = 0; i < ltss.size(); i++) {
			for (const auto& t : ltss[i].states_.at(states_vec[i]).transitions_) {
				std::vector<std::string> next_states = states_vec;
				next_states[i] = t.second;
				combined_lts.AddTransition(states_str, t.first, StateVectorToString(next_states));
				CombineRecursive(ltss, next_states, visited, combined_lts);
			}
		}
	}

	/*
	 * @brief Transforms a vector of states into a singular state string (_,_,_,_)
	 */
	std::string StateVectorToString(const std::span<std::string>& vec) {
		std::string ret;
		for (uint_fast8_t i = 0; i < vec.size(); i++) {
			ret += vec[i];
			if (i != vec.size() - 1) {
				ret += ',';
			}
		}
		return ret;
	}

}