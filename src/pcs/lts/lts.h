#pragma once

#include <unordered_map>
#include <string>
#include <ostream>
#include <fstream>
#include <filesystem>

#include "pcs/lts/state.h"

namespace pcs::lts {

	template <typename TransitionT = std::string>
	class LabelledTransitionSystem {
	public:
		using State = State<TransitionT>;
		std::unordered_map<std::string, State> states_; // @Todo: unordered map is inefficent STL container
	private:
		std::string initial_state_;
	public:
		LabelledTransitionSystem();
		LabelledTransitionSystem(const std::string& initial_state, bool create_initial);
		LabelledTransitionSystem(const std::filesystem::path& filepath);
		~LabelledTransitionSystem();

		void SetInitialState(const std::string& state, bool create_if_not_exists);
		std::string GetInitialState() const;

		size_t NumberOfStates() const;
		bool HasState(const std::string& key) const;
		bool AddState(const State& state);
		bool AddState(State&& state);
		bool RemoveState(const std::string& key);

		void AddTransition(const std::string& start_state, const TransitionT label,
			const std::string& end_state, bool create_missing_states=true);

		bool operator==(const LabelledTransitionSystem& other) const;
		State& operator[](const std::string& key);
		const State& operator[](const std::string& key) const;

		friend std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem& lts);
		friend std::ofstream& operator<<(std::ofstream& os, const LabelledTransitionSystem& lts);
	};
}