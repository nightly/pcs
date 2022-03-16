#include "pcs/lts/lts.h"

#include <utility>
#include <algorithm>
#include <string>
#include <filesystem>
#include <ostream>

#include "pcs/lts/parser.h"

namespace pcs::lts {

	/*
	 * @brief Labelled Transition System (LTS) constructor
	 */
	LabelledTransitionSystem::LabelledTransitionSystem() {}

	/*
	 * @brief Constructor with overload for setting initial state
	 * @param initial_state: string key of initial state
	 * @param create_initial: will create the initial lts::State object if true
	 */
	LabelledTransitionSystem::LabelledTransitionSystem(const std::string& initial_state, bool create_initial) {
		SetInitialState(initial_state, create_initial);
	}

	/*
	 * @brief LTS constructor overloaded to read file input into a LTS
	 * @param filepath: file in form of data/ (also specified in ReadFromFile)
	 */
	LabelledTransitionSystem::LabelledTransitionSystem(const std::filesystem::path& filepath) {
		ReadFromFile(*this, filepath);
	}

	/*
	 * @brief Sets the initial state of the LTS
	 * @param create_if_not_exists: will create the lts::State object for initial state unless it already exists
	 */
	void LabelledTransitionSystem::SetInitialState(const std::string& state, bool create_if_not_exists) {
		if (!HasState(state) && create_if_not_exists) {
			AddState(pcs::lts::State(state));
		}
		initial_state_ = state;
	}

	/*
	 * @brief Returns the initial state of the LTS
	 */
	std::string LabelledTransitionSystem::GetInitialState() const {

		return initial_state_;
	}
	
	size_t LabelledTransitionSystem::NumberOfStates() const {
		return states_.size();
	}

	/*
	 * @brief Determines whether a named state exists within the LTS
	 */
	bool LabelledTransitionSystem::HasState(const std::string& key) const {
		return states_.contains(key);
	}

	/*
	 * @brief Adds a new state to the LTS providing it doesn't exist in the HashMap already
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
	 * @brief Adds a new state with move semantics
	 * @returns False if state already exists, true if it doesn't.
	 */
	bool LabelledTransitionSystem::AddState(lts::State&& state) {
		if (!HasState(state.GetName())) {
			states_.emplace(std::make_pair(state.GetName(), std::move(state)));
		}
		return false;
	}

	/*
	* @brief Removes a state if it exists within the LTS
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
	 * @brief Adds an arbitrary transition to the LTS
	 *
	 * If the initial and/or end state don't exist, they will be created
	 */
	void LabelledTransitionSystem::AddTransition(const std::string& start_state, const std::string& label, const std::string& end_state) {
		if (!HasState(start_state)) {
			AddState(pcs::lts::State(start_state));
		}

		State& s = states_.at(start_state);
		s.AddTransistion(label, end_state);

		if (!HasState(end_state)) {
			AddState(pcs::lts::State(end_state));
		}
	}

	/*
	 * @brief Equality operator for LTS, mainly useful for testing
	 * @returns Whether the given LTS is equivalent to the other specified one
	 */
	bool LabelledTransitionSystem::operator==(const LabelledTransitionSystem& other) const {
		return (initial_state_ == other.initial_state_) && (states_ == other.states_);
	}

	/*
	 * @brief Operator[] overload to access a State object at any given key
	 */
	State& LabelledTransitionSystem::operator[](const std::string& key) {
		return states_[key];
	}

	/*
	  * @brief Operator[] overload to access a State object at given key in a const-manner 
	  */
	const State& LabelledTransitionSystem::operator[](const std::string& key) const {
		return states_.at(key);
	}

	/*
	 * @brief Destructor for LTS
	 */
	LabelledTransitionSystem::~LabelledTransitionSystem() {
		states_.clear();
	}

}
