#pragma once

#include <unordered_map>
#include <string>
#include <ostream>
#include <fstream>
#include <vector>

#include "pcs/lts/state.h"
#include "pcs/common/vec_comma_ostream.h"

namespace pcs {

	/*
	 * @brief Labelled Transition System: comprised of states and transitions to other states 
	 * @tparam KeyT: the data type used to defined keys/state names. E.g std::string, std::vector<std::string>
	 * @tparam TransitionT : defines the data type used to represent transitions. E.g. std::string, CompositeOperation
	 */
	template <typename KeyT = std::string, typename TransitionT = std::string>
	class LTS {
	public:
		using State = internal::State<KeyT, TransitionT>;
	private:
		std::unordered_map<KeyT, State> states_;
		KeyT initial_state_;
	public:
		LTS() = default;

		LTS(const KeyT& initial_state, bool create_initial) {
			SetInitialState(initial_state, create_initial);
		}

		~LTS() = default;

		const std::unordered_map<KeyT, State>& states() const {
			return states_;
		}

		KeyT initial_state() const {
			return initial_state_;
		}

		void set_initial_state(const KeyT& state, bool create_if_not_exists=true) {
			if (!HasState(state) && create_if_not_exists) {
				AddState(State(state));
			}
			initial_state_ = state;
		}

		bool HasState(const KeyT& key) const {
			return states_.contains(key);
		}

		size_t NumOfStates() const {
			return states_.size();
		}

		/*
		 * @brief Removes a given state but allows dangling transitions to exist from other states to the deleted state 
		 */
		bool RemoveStateSoft(const KeyT& key) {
			if (HasState(key)) {
				states_.erase(key);
				return true;
			}
			return false;
		}

		/*
		 * @brief Removes a given state and then checks for dangling transitions with all other states
		 */
		bool RemoveStateFull(const KeyT& key) {
			if (HasState(key)) {
				states_.erase(key);
				for (auto& [k, v] : states_) {
					typename std::vector<std::pair<TransitionT, KeyT>>::iterator it = v.transitions_.begin();
					while (it != v.transitions_.end()) {
						if (it->second == key) {
							v.transitions_.erase(it++);
						} else {
							++it;
						}
					}
				}
				return true;
			}
			return false;
		}

		void AddTransition(const KeyT& start_state, const TransitionT& label, const KeyT& end_state, 
		bool create_missing_states = true) {
			if (!HasState(start_state) && create_missing_states) {
				AddState(State(start_state));
			}
			if (!HasState(end_state) && create_missing_states) {
				AddState(State(end_state));
			}

			State& s = states_.at(start_state);
			s.AddTransition(label, end_state);
		}

		bool operator==(const LTS& other) const {
			return (initial_state_ == other.initial_state_) && (states_ == other.states_);
		}

		State& operator[](const KeyT& key) {
			return states_[key];
		}

		const State& operator[](const KeyT& key) const {
			return states_.at(key);
		}

		friend std::ostream& operator<<(std::ostream& os, const LTS& lts) {
			if (lts.initial_state_.empty() && lts.states_.empty()) {
				os << "Empty Labelled Transition System\n";
				return os;
			}
			os << "Initial state: " << lts.initial_state_ << '\n';
			for (const auto& pair : lts.states_) {
				os << pair.second;
			}
			return os;
		}

		friend std::ofstream& operator<<(std::ofstream& os, const LTS& lts) {
			os << "digraph finite_state_machine {\n";
			os << "	fontname=\"Helvetica, Arial, sans - serif\"\n";
			os << "	node [fontname=\"Helvetica, Arial, sans - serif\"]\n";
			os << "	edge [fontname=\"Helvetica, Arial, sans - serif\"]\n";
			os << "	rankdir=LR;\n";
			os << "	node [shape = doublecircle];\n";
			os << "	" << "\"" << lts.initial_state_ << "\"" << ";\n";
			os << "	node [shape = circle];\n";
			for (const auto& pair : lts.states_) {
				os << pair.second;
			}
			os << "}";
			return os;
		}
	private:
		bool AddState(State&& state) {
			if (!HasState(state.name())) {
				states_.emplace(std::make_pair(state.name(), std::move(state)));
				return true;
			}
			return false;
		}

		bool AddState(const State&& state) {
			if (!HasState(state.name())) {
				states_.emplace(std::make_pair(state.name(), state));
				return true;
			}
			return false;
		}
	};
}

/* 
    @Todo: `#include vec_ostream` should be replaced with a template writer specialization of LTS/State themselves
	@Todo: unordered map is an inefficent STL container
*/
