#pragma once

#include <unordered_map>
#include <string>
#include <ostream>
#include <fstream>
#include <filesystem>

#include "pcs/lts/state.h"

namespace pcs {

	template <typename TransitionT = std::string>
	class LabelledTransitionSystem {
	public:
		using State = State<TransitionT>;
		std::unordered_map<std::string, State> states_;
	private:
		std::string initial_state_;
	public:
		
		/* @tparam TransitionT : defines the data type used to represent transitions. E.g. std::string, CompositeOperation
		*/
		LabelledTransitionSystem() = default;

		LabelledTransitionSystem(const std::string& initial_state, bool create_initial) {
			SetInitialState(initial_state, create_initial);
		}

		~LabelledTransitionSystem() = default;

		std::string GetInitialState() const {
			return initial_state_;
		}

		void SetInitialState(const std::string& state, bool create_if_not_exists) {
			if (!HasState(state) && create_if_not_exists) {
				AddState(State(state));
			}
			initial_state_ = state;
		}

		bool HasState(const std::string& key) const {
			return states_.contains(key);
		}

		size_t NumberOfStates() const {
			return states_.size();
		}

		bool AddState(State&& state) {
			if (!HasState(state.GetName())) {
				states_.emplace(std::make_pair(state.GetName(), state));
				return true;
			}
			return false;
		}

		bool AddState(const State& state) {
			if (!HasState(state.GetName())) {
				states_.emplace(std::make_pair(state.GetName(), std::move(state)));
			}
			return false;
		}

		bool RemoveState(const std::string& key) {
			if (HasState(key)) {
				states_.erase(key);
				return true;
			}
			return false;
		}

		void AddTransition(const std::string& start_state, const TransitionT label,
		const std::string& end_state, bool create_missing_states = true) {
			if (!HasState(start_state) && create_missing_states) {
				AddState(State(start_state));
			}
			if (!HasState(end_state) && create_missing_states) {
				AddState(State(end_state));
			}

			State& s = states_.at(start_state);
			s.AddTransistion(label, end_state);
		}

		bool operator==(const LabelledTransitionSystem& other) const {
			return (initial_state_ == other.initial_state_) && (states_ == other.states_);
		}

		State& operator[](const std::string& key) {
			return states_[key];
		}

		const State& operator[](const std::string& key) const {
			return states_.at(key);
		}

		friend std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem& lts) {
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

		friend std::ofstream& operator<<(std::ofstream& os, const LabelledTransitionSystem& lts) {
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

	};
}

/*  @Todo: further templatize with KeyT
	@Todo: AddState methods should be private and the public interface should be accessed through only adding transitions,
		allowing create_missing_states variables also to be removed
	@Todo: unordered map is an inefficent STL container
*/
