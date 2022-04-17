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

		std::vector<std::pair<TransitionT, KeyT>>& transitions() {
			return transitions_;
		}

		/*
		 * @brief Adds a given transition to the State object, first checking whether or not that exact transition exists or not
		 */
		void AddTransition(const TransitionT& label, const KeyT& end_state) {
			// @Performance: could be a redundant check, or an alternative function could be written
			if (!TransitionExists(label, end_state)) {
				std::pair<TransitionT, KeyT> transistion = std::make_pair(label, end_state);
				transitions_.emplace_back(transistion);
			}
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
			return (name_ == other.name_) && (transitions_ == other.transitions_);
		}

		template <typename _KeyT, typename _TransitionT>
		friend std::ostream& operator<<(std::ostream& os, const State& state);

		template <typename _KeyT, typename _TransitionT>
		friend std::ofstream& operator<<(std::ofstream& os, const State& state);
	};

}
