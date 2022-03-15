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
		combined_lts.AddState(initial_key);
		combined_lts.SetInitialState(initial_key);

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
	pcs::lts::merge::merge()
	{

	}

	void pcs::lts::merge::createVisitedDictionary(std::vector<LabelledTransitionSystem> lts, std::map<std::string, bool>& isVisited)
	{
		std::set<std::string> combination;
		for (auto ele : lts.at(0).getStates()) {
			combination.insert(ele.first);
		}

		for (int i = 1; i < lts.size(); i++) {
			std::vector<std::string> temp(combination.begin(), combination.end());
			combination.clear();
			for (int j = 0; j < temp.size(); j++) {
				for (auto ele : lts.at(i).getStates()) {
					combination.insert(temp[j] + ele.first);
				}
			}
		}
		/*
		 * @brief Transforms a vector of states into a singular state string (_,_,_,_)
		 */

		for (auto i : combination) {
			isVisited.insert(std::pair<std::string, bool>(i.c_str(), false));
		}
	}

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

	void pcs::lts::merge::merger(std::vector<LabelledTransitionSystem> lts)
	{
		std::map<std::string, bool> isVisited;
		std::vector<std::string> currState;

		for (int i = 0; i < lts.size(); i++) {
			currState.push_back(lts.at(i).getInitialState());
		}

		createVisitedDictionary(lts, isVisited);
		combine(lts, currState, isVisited);
	}

	void pcs::lts::merge::combine(std::vector<LabelledTransitionSystem> lts, std::vector<std::string> currState, std::map<std::string, bool>& isVisited)
	{
		std::string decision = "";
		for (int i = 0; i < currState.size(); i++) {
			decision += currState[i];
		}
		if (isVisited.at(decision)) {
			return;
		}
		isVisited[decision] = true;

		for (int i = 0; i < currState.size(); i++) {
			for (auto ele : lts[i].getStates()) {
				if (ele.first == currState[i]) {
					std::vector<std::vector<std::string>> currstatecopies;
					for (int j = 0; j < ele.second.transitions_.size(); j++) {
						std::vector<std::string> copy = currState;
						copy[i] = ele.second.transitions_[j].first;
						std::string finaldecision = "";
						for (int i = 0; i < copy.size(); i++) {
							finaldecision += copy[i];
						}
						std::cout << decision << "->" << ele.second.transitions_[j].second << "->" << finaldecision << std::endl;
						currstatecopies.push_back(copy);
					}
					for (auto copy : currstatecopies) {
						combine(lts, copy, isVisited);
					}
				}
			}
		}
	}
}
