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

		/* @tparam TransitionT : defines the data type used to represent transitions. E.g. std::string, CompositeOperation 
		 */
		State() = default;

		State(const std::string& name) 
			: name_(name) {}

		~State() = default;

		std::string GetName() const {
			return name_;
		}

		void AddTransistion(const TransitionT& label, const std::string& end_state) {
			std::pair<TransitionT, std::string> transistion = std::make_pair(label, end_state);
			transitions_.emplace_back(transistion);
		}

		bool TransistionExists(const TransitionT& label, const std::string& end_state) const {
			for (const auto& t : transitions_) {
				if ((t.first == label) && (t.second == end_state)) {
					return true;
				}
			}
			return false;
		}

		bool IsEmpty() const {
			return transitions_.empty();
		}

		bool operator==(const State& other) const {
			return (name_ == other.name_) && (transitions_ == other.transitions_);
		}

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