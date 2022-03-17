#include "pcs/lts/state.h"

#include <string>
#include <ostream>

namespace pcs::lts {

	/*
	 * @brief State constructor, representing a single state within the LTS and its transitions. 
	 * @param name: given name of the State. This is represented by a LTS as a key within the HashMap.
	 */
	State::State(const std::string& name) 
		: name_(name) {
	}

	/*
	 * @brief Gets the name of the given state
	 * @returns Name of the state — which also corresponds to the name within the hashmap
	 */
	std::string State::GetName() const {
		return name_;
	}

	/*
	 * @brief Adds a transitition to the given state.
	 * @param label: label for transitition. Special cases: nop, in, out.
	 * @param end_state: the state the transition ends at/points to.
	 */
	void State::AddTransistion(const std::string& label, const std::string& end_state) {
		std::pair<std::string, std::string> transistion = std::make_pair(label, end_state);
		transitions_.emplace_back(transistion);
	}

	/*
	 * @brief Returns whether or not the State is empty (which is defined by having no transitions) 
	 */
	bool State::IsEmpty() const {
		return transitions_.empty();
	}

	/* 
	 * @brief Compares two State objects for equality
	 * @returns True if both objects are equal, false otherwise
	 */
	bool State::operator==(const State& other) const {
		return (name_ == other.name_) && (transitions_ == other.transitions_);
	}

	/*
	 * @brief State destructor 
	 */
	State::~State() {
		transitions_.clear();
	}

}