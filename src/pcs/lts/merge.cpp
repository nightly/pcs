#include "pcs/lts/merge.h"

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
		std::vector<std::string> state_strings;
		std::vector<std::tuple<std::string, std::string, std::string>> transitions;
		for (const auto& lts : ltss) {
			// Populate with initial states
			state_strings.emplace_back(lts.GetInitialState());
		}
		std::string initial_key = StateVectorToString(state_strings);
		combined_lts.SetInitialState(initial_key, true);

		CombineRecursive(ltss, state_strings, combined_lts);

		return combined_lts;
	}

	void CombineRecursive(const std::span<LabelledTransitionSystem>& ltss, std::vector<std::string>& state_strings,
		LabelledTransitionSystem& combined_lts) {

		for (uint_fast8_t i = 0; i < ltss.size(); i++) {
			for (const auto& t : ltss[i].states_.at(state_strings[i]).transitions_) {
				combined_lts.AddTransition(StateVectorToString(state_strings), t.second, t.first);
				state_strings[i] = t.first;
				CombineRecursive(ltss, state_strings, combined_lts);
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