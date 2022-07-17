#pragma once 

#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <utility>

#include "pcs/lts/transition.h"

namespace pcs {

	template <typename KeyT = std::string, typename TransitionT = std::string>
	class State {
	public:
		std::vector<Transition<TransitionT, KeyT>> transitions_;
	public:

		/** 
		 * @tparam KeyT: the data type used to defined keys/state names. E.g std::string, std::vector<std::string>
		 * @tparam TransitionT : defines the data type used to represent transitions. E.g. std::string, CompositeOperation 
		 */
		State() = default;

		~State() = default;

		std::vector<Transition<TransitionT, KeyT>>& transitions() {
			return transitions_;
		}

		const std::vector<Transition<TransitionT, KeyT>>& transitions() const {
			return transitions_;
		}

		void AddTransition(const TransitionT& label, const KeyT& end_state) {
			transitions_.emplace_back(label, end_state);
		}

		bool TransitionExists(const TransitionT& label, const KeyT& end_state) const {
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
			return (transitions_ == other.transitions_);
		}

		template <typename _KeyT, typename _TransitionT>
		friend std::ostream& operator<<(std::ostream& os, const State& state);

		template <typename _KeyT, typename _TransitionT>
		friend std::ofstream& operator<<(std::ofstream& os, const State& state);
	};

}
