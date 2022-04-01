#include "pcs/lts/state.h"

#include <string>
#include <ostream>

#include "pcs/operation/composite.h"

namespace pcs {

	/*
	 * @brief State constructor, representing a single state within the LTS and its transitions. 
	 * @param name: given name of the State. This is represented by a LTS as a key within the HashMap.
	 * @tparam TransitionT: defines the data type used to represent transitions. Defaults to std::string.
	 */
	template <typename TransitionT>
	State<TransitionT>::State(const std::string& name)
		: name_(name) {
	}

	/*
	 * @brief Gets the name of the given state
	 * @returns Name of the state — which also corresponds to the name within the hashmap
	 */
	template <typename TransitionT>
	std::string State<TransitionT>::GetName() const {
		return name_;
	}

	/*
	 * @brief Adds a transitition to the given state
	 * @param label: label for transitition. Special cases: nop, in, out.
	 * @param end_state: the state the transition ends at/points to.
	 */
	template <typename TransitionT>
	void State<TransitionT>::AddTransistion(const TransitionT& label, const std::string& end_state) {
		std::pair<TransitionT, std::string> transistion = std::make_pair(label, end_state);
		transitions_.emplace_back(transistion);
	}

	/*
	 * @brief Returns whether or not a given state has a transistion that matches the one specified
	 */
	template <typename TransitionT>
	bool State<TransitionT>::TransistionExists(const TransitionT& label, const std::string& end_state) const {
		for (const auto& t : transitions_) {
			if ((t.first == label) && (t.second == end_state)) {
				return true;
			}
		}
		return false;
	}


	/*
	 * @brief Returns whether or not the State is empty (which is defined by having no transitions) 
	 */
	template <typename TransitionT>
	bool State<TransitionT>::IsEmpty() const {
		return transitions_.empty();
	}

	/* 
	 * @brief Compares two State objects for equality
	 * @returns True if both objects are equal, false otherwise
	 */
	template <typename TransitionT>
	bool State<TransitionT>::operator==(const State& other) const {
		return (name_ == other.name_) && (transitions_ == other.transitions_);
	}

	/*
	 * @brief State destructor 
	 */
	template <typename TransitionT>
	State<TransitionT>::~State() {
		transitions_.clear();
	}

	/* Explicit template instantiations */
	template class State<std::string>;
	template class State<CompositeOperation>;

}