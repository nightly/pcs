#pragma once

#include <unordered_map>
#include <string>

#include "state.h"

namespace pcs::lts {
	
	class LabelledTransitionSystem {
	private:
		std::unordered_map<std::string, lts::State> states_; // @Todo: unordered_map is an inefficent STL container,
		std::string initial_state_;
		std::string end_state_;
	public:
		LabelledTransitionSystem();
		~LabelledTransitionSystem();

		void SetInitialState(const std::string& state);
		bool HasState(const std::string& key) const;
		bool AddState(lts::State& state);
		bool AddState(lts::State&& state);
		bool RemoveState(const std::string& key);

		void AddSegment(const std::string& start_state, const std::string& label, const std::string& end_state);

		bool operator==(const LabelledTransitionSystem& other) const;
	};
}