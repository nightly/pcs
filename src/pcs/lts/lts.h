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
		std::unordered_map<std::string, State> states_; // @Todo: unordered map is inefficent STL container
	private:
		std::string initial_state_;
	public:
		LabelledTransitionSystem() = default;
		LabelledTransitionSystem(const std::string& initial_state, bool create_initial);
		LabelledTransitionSystem(const std::filesystem::path& filepath);
		~LabelledTransitionSystem();

		void SetInitialState(const std::string& state, bool create_if_not_exists);
		std::string GetInitialState() const;

		size_t NumberOfStates() const;
		bool HasState(const std::string& key) const;
		bool AddState(State&& state);
		bool AddState(const State& state);
		bool RemoveState(const std::string& key);

		void AddTransition(const std::string& start_state, const TransitionT label,
			const std::string& end_state, bool create_missing_states=true);

		bool operator==(const LabelledTransitionSystem& other) const;
		State& operator[](const std::string& key);
		const State& operator[](const std::string& key) const;

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
	};
}