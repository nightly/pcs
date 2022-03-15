#pragma once

#include <unordered_map>
#include <string>
#include <ostream>
#include <filesystem>

#include "state.h"

namespace pcs::lts {
	
	class LabelledTransitionSystem {
	public: 
		// @Todo: private data members + replace unordered_map
		std::unordered_map<std::string, lts::State> states_; 
		std::string initial_state_;
	public:
		LabelledTransitionSystem();
		LabelledTransitionSystem(const std::filesystem::path& filepath);
		~LabelledTransitionSystem();

		void SetInitialState(const std::string& state);
		std::string GetInitialState() const;
		bool HasState(const std::string& key) const;
		bool AddState(lts::State& state);
		bool AddState(lts::State&& state);
		bool RemoveState(const std::string& key);

		void AddTransition(const std::string& start_state, const std::string& label, const std::string& end_state);

		bool operator==(const LabelledTransitionSystem& other) const;
		friend std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem& lts);
	};
}