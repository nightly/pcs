#include "pcs/lts/lts.h"

#include <utility>
#include <algorithm>

namespace pcs::lts {
	
	/*
	 * @brief Labelled Transition System constructor.
	 */
	LabelledTransitionSystem::LabelledTransitionSystem() {

	}

	/*
	 * @brief Sets the initial state of the LTS. 
	 */
	void LabelledTransitionSystem::SetInitialState(const std::string& state) {
		initial_state_ = state;
	}

	bool LabelledTransitionSystem::HasState(const std::string& key) const {
		return states_.contains(key);
	}
	
	/*
	 * @brief Adds a new state to the LTS providing it doesn't exist in the HashMap already. 
	 * @returns False if state already exists, true if it doesn't.
	 */
	bool LabelledTransitionSystem::AddState(lts::State& state) {
		if (!HasState(state.GetName())) {
			states_.emplace(std::make_pair(state.GetName(), state));
			return true;
		}
		return false;
	}

	/*
	 * @brief Adds a new state with move semantics.
	 * @returns False if state already exists, true if it doesn't.
	 */
	bool LabelledTransitionSystem::AddState(lts::State&& state) {
		if (!HasState(state.GetName())) {
			states_.emplace(std::make_pair(state.GetName(), std::move(state)));
		}
		return false;
	}
	
	/*
	* @brief Removes a state if it exists within the LTS.
	* @returns False if a removal cannot occur (because the state isn't present), true if successful
	*/
	bool LabelledTransitionSystem::RemoveState(const std::string& key) {
		if (HasState(key)) {
			states_.erase(key);
			return true;
		} 
		return false;
	}

	/*
	 * @brief Adds a segment to the Labelled Transition System.
	 * 
	 * This is composed of the form based on the three parameters:
	 * sx ay sz
	 * where sx = start state, ay = label/action, sz = end state
	 * 
	 * If the initial state doesn't exist, it will first be created.
	 */
	void LabelledTransitionSystem::AddSegment(const std::string& start_state, const std::string& label, const std::string& end_state) {
		if (!HasState(start_state)) {
			states_.emplace(std::make_pair(start_state, State(start_state)));
		}

		State& s = states_.at(start_state);
		s.AddTransistion(label, end_state);

		if (!HasState(end_state)) {
			states_.emplace(std::make_pair(end_state, State(start_state)));

		}
	}

	/*
	 * @brief Equality operator for LTS, useful for testing purposes
	 * @returns Whether the given LTS is equivalent to the other specified one
	 */
	bool LabelledTransitionSystem::operator==(const LabelledTransitionSystem& other) const {
		return (initial_state_ == other.initial_state_) && (states_.size() == other.states_.size()) &&
			std::equal(states_.begin(), states_.end(), other.states_.begin());
	}


	/*
	 * @brief Destructor for LTS 
	 */
	LabelledTransitionSystem::~LabelledTransitionSystem() {
		states_.clear();
	}

}