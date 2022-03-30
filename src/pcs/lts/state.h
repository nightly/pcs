#pragma once 

#include <string>
#include <vector>
#include <ostream>
#include <utility>

namespace pcs {

	template <typename TransitionT = std::string>
	class State {
	public:
		std::vector<std::pair<TransitionT, std::string>> transitions_;
	private:
		std::string name_;
	public:
		State() = default;
		State(const std::string& name);
		~State();

		std::string GetName() const;
		void AddTransistion(const TransitionT& label, const std::string& end_state);
		bool TransistionExists(const TransitionT& label, const std::string& end_state) const;
		bool IsEmpty() const;

		bool operator==(const State& other) const;

		friend std::ostream& operator<<(std::ostream& os, const State& state);
		friend std::ofstream& operator<<(std::ofstream& os, const State& state);
	};

}