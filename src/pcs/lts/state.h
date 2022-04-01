#pragma once 

#include <string>
#include <vector>
#include <ostream>
#include <fstream>
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

		friend std::ostream& operator<<(std::ostream& os, const State& state) {
			os << "State name: " << state.GetName() << '\n';
			if (state.transitions_.empty()) {
				os << "  With 0 transitions" << '\n';
				return os;
			}
			os << "  Transitions: " << '\n';
			for (const auto& pair : state.transitions_) {
				os << "    Label: " << pair.first << " " << "End State: " << pair.second << '\n';
			}
			return os;
		}
		friend std::ofstream& operator<<(std::ofstream& os, const State& state) {
			if (state.IsEmpty()) {
				os << "	" << "\"" << state.name_ << "\"" << "\n";
			}
			for (const auto& t : state.transitions_) {
				os << "	" << "\"" << state.name_ << "\"" << " -> " << "\"" << t.second << "\"" << " [label = " << "\"";
				os << t.first << "\"];\n";
			}
			return os;
		}
	};

}