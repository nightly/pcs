#pragma once 

#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <utility>

namespace pcs::internal {

	template <typename KeyT, typename TransitionT = std::string>
	class State {
	public:
		std::vector<std::pair<TransitionT, KeyT>> transitions_;
	private:
		KeyT name_;
	public:

		/* 
		 * @tparam KeyT: the data type used to defined keys/state names. E.g std::string, std::vector<std::string>
		 * @tparam TransitionT : defines the data type used to represent transitions. E.g. std::string, CompositeOperation 
		 */
		State() = default;

		State(const KeyT& name) 
			: name_(name) {}

		~State() = default;

		KeyT name() const {
			return name_;
		}

		void AddTransition(const TransitionT& label, const KeyT& end_state) {
			std::pair<TransitionT, KeyT> transistion = std::make_pair(label, end_state);
			transitions_.emplace_back(transistion);
		}

		bool TransistionExists(const TransitionT& label, const KeyT& end_state) const {
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
			os << "State name: " << state.name_ << '\n';
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

/* 
	@Todo: internal namespace
*/
