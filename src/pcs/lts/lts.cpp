#include "pcs/lts/lts.h"

#include <utility>
#include <algorithm>
#include <string>
#include <filesystem>
#include <ostream>

#include "pcs/lts/parsers.h"
#include "pcs/operation/composite.h"
#include "pcs/product/parser.h"

namespace pcs::lts {

	/*
	 * @brief Labelled Transition System (LTS) constructor
	 */
	template <typename TransitionT>
	LabelledTransitionSystem<TransitionT>::LabelledTransitionSystem() {}

	/*
	 * @brief Constructor with overload for setting initial state
	 * @param initial_state: string key of initial state
	 * @param create_initial: will create the initial lts::State object if true
	 */
	template <typename TransitionT>
	LabelledTransitionSystem<TransitionT>::LabelledTransitionSystem(const std::string& initial_state, bool create_initial) {
		SetInitialState(initial_state, create_initial);
	}

	/*
	 * @brief LTS constructor overloaded to read file input into a LTS (template specialization for std::string)
	 * @param filepath: file in form of data/ (also specified in ReadFromFile)
	 */
	template <>
	LabelledTransitionSystem<std::string>::LabelledTransitionSystem(const std::filesystem::path& filepath) {
		ReadFromFile(*this, filepath);
	}

	/*
	 * @brief LTS constructor overloaded to read file input into a LTS (template specialization for CompositeOperation)
	 * @param filepath: JSON filepath
	 */
	template <>
	LabelledTransitionSystem<CompositeOperation>::LabelledTransitionSystem(const std::filesystem::path& filepath) {
		pcs::product::ReadFromJsonFile(*this, filepath);
	}

	/*
	 * @brief Sets the initial state of the LTS
	 * @param create_if_not_exists: will create the lts::State object for initial state unless it already exists
	 */
	template <typename TransitionT>
	void LabelledTransitionSystem<TransitionT>::SetInitialState(const std::string& state, bool create_if_not_exists) {
		if (!HasState(state) && create_if_not_exists) {
			AddState(pcs::lts::State<TransitionT>(state));
		}
		initial_state_ = state;
	}

	/*
	 * @brief Returns the initial state of the LTS
	 */
	template <typename TransitionT>
	std::string LabelledTransitionSystem<TransitionT>::GetInitialState() const {
		return initial_state_;
	}
	
	/*
	 * @brief Returns the number of states contained within the LTS. 
	 */
	template <typename TransitionT>
	size_t LabelledTransitionSystem<TransitionT>::NumberOfStates() const {
		return states_.size();
	}

	/*
	 * @brief Determines whether a named state exists within the LTS
	 */
	template <typename TransitionT>
	bool LabelledTransitionSystem<TransitionT>::HasState(const std::string& key) const {
		return states_.contains(key);
	}

	/*
	 * @brief Adds a new state to the LTS providing it doesn't exist in the HashMap already
	 * @returns False if state already exists, true if it doesn't.
	 */
	template <typename TransitionT>
	bool LabelledTransitionSystem<TransitionT>::AddState(const State& state) {
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
	template <typename TransitionT>
	bool LabelledTransitionSystem<TransitionT>::AddState(State&& state) {
		if (!HasState(state.GetName())) {
			states_.emplace(std::make_pair(state.GetName(), std::move(state)));
		}
		return false;
	}

	/*
	* @brief Removes a state if it exists within the LTS
	* @returns False if a removal cannot occur (because the state isn't present), true if successful
	*/
	template <typename TransitionT>
	bool LabelledTransitionSystem<TransitionT>::RemoveState(const std::string& key) {
		if (HasState(key)) {
			states_.erase(key);
			return true;
		}
		return false;
	}

	/*
	 * @brief Adds an arbitrary transition to the LTS - allows duplicate transitions
	 *
	 * @params start_state, label, end_state: the starting transition, the action/label, and the ending state.
	 * @param create_missing_states: if the start/end state don't exist, they will be created if true. Default: true.
	 */
	template <typename TransitionT>
	void LabelledTransitionSystem<TransitionT>::AddTransition(const std::string& start_state, const TransitionT label,
	const std::string& end_state, bool create_missing_states) {
		if (!HasState(start_state) && create_missing_states) {
			AddState(pcs::lts::State<TransitionT>(start_state));
		}
		if (!HasState(end_state) && create_missing_states) {
			AddState(pcs::lts::State<TransitionT>(end_state));
		}

		State& s = states_.at(start_state);
		s.AddTransistion(label, end_state);
	}

	/*
	 * @brief Operator== for LTS, mainly useful for testing
	 * @returns Whether the given LTS is equivalent to the other specified one
	 */
	template <typename TransitionT>
	bool LabelledTransitionSystem<TransitionT>::operator==(const LabelledTransitionSystem& other) const {
		return (initial_state_ == other.initial_state_) && (states_ == other.states_);
	}

	/*
	 * @brief Operator[] overload to access a State object at any given key
	 */
	template <typename TransitionT>
	State<TransitionT>& LabelledTransitionSystem<TransitionT>::operator[](const std::string& key) {
		return states_[key];
	}

	/*
	  * @brief Operator[] overload to access a State object at given key in a const-manner 
	  */
	template <typename TransitionT>
	const State<TransitionT>& LabelledTransitionSystem<TransitionT>::operator[](const std::string& key) const {
		return states_.at(key);
	}

	/*
	 * @brief Destructor for LTS
	 */
	template <typename TransitionT>
	LabelledTransitionSystem<TransitionT>::~LabelledTransitionSystem() {
		states_.clear();
	}

	/* Explicit template instantiations */
	template class LabelledTransitionSystem<std::string>;
	template class LabelledTransitionSystem<CompositeOperation>;

}
