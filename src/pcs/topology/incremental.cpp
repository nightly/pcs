#include "pcs/topology/incremental.h"

#include <vector>
#include <span>
#include <string>

#include <iostream>

#include <spdlog/fmt/ranges.h>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"

namespace pcs {

	IncrementalTopology::IncrementalTopology(const std::span<LTS<std::string, std::string>>& ltss)
	: ltss_(ltss) { 
		// We start our incremental topology by expanding the initial state at the beginning because we want to set the initial state anyway.
		std::vector<std::string> initial_key;
		initial_key.reserve(ltss.size());
		for (const auto& lts : ltss) {
			initial_key.emplace_back(lts.initial_state());
		}
		topology_.set_initial_state(initial_key);
		ExpandState(initial_key);
		visited_.insert(initial_key);
	}

	
	const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& IncrementalTopology::lts() const {
		return topology_;
	}

	const std::vector<std::string>& IncrementalTopology::initial_state() const {
		return topology_.initial_state();
	}

	const State<std::vector<std::string>, std::pair<size_t, std::string>>& IncrementalTopology::at(const std::vector<std::string>& key) {
		return topology_.initial_state(); // @Testing

		// @Performance: topology_.contains() and in ExpandState use false for create_missing_states, then manually add state here (?)
		if (visited_.contains(key) == true) {
			return topology_.states().at(key);
		} else {
			visited_.insert(key);
			ExpandState(key);
			return topology_.states().at(key);
		}
	}

	/**
	 * @brief ExpandState functions similar to CombineRecursive but it will not recursively expand itself.
	 * We apply the transitions that are found from the given state key based on any of the current resources.
	 */
	void IncrementalTopology::ExpandState(const std::vector<std::string>& key) {
		PCS_INFO("Processing {}", fmt::join(key, ","));

		for (size_t i = 0; i < ltss_.size(); ++i) {
			for (const auto& transition : ltss_[i].states().at(key[i]).transitions_) {
				if ((transition.first.find("in:") != std::string::npos) || (transition.first.find("out:") != std::string::npos)) {
					std::optional<std::vector<std::string>> transfer_state = MatchingTransfer(ltss_, key, i, transition);
					if (!transfer_state.has_value()) {
						continue;
					}
					topology_.AddTransition(key, std::make_pair(i, transition.first), *transfer_state, true);
				} else {
					std::vector<std::string> next_states = key;
					next_states[i] = transition.second;
					topology_.AddTransition(key, std::make_pair(i, transition.first), next_states, true);
				}
			}
		}
	}

}