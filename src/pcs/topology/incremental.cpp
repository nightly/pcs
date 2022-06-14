#include "pcs/topology/incremental.h"

#include <vector>
#include <span>
#include <string>

#include <spdlog/fmt/ranges.h>

#include "pcs/lts/lts.h"
#include "pcs/common/log.h"

namespace pcs {

	IncrementalTopology::IncrementalTopology(const std::vector<LTS<std::string, std::string>>& ltss)
	: ltss_(ltss) { 
		// We start our incremental topology by setting the initial state. We also expand the initial state.
		std::vector<std::string> initial_key;
		initial_key.reserve(ltss_.size());
		for (const auto& lts : ltss_) {
			initial_key.emplace_back(lts.initial_state());
		}
		topology_.set_initial_state(initial_key, true);
		ExpandState(initial_key);
	}

	const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& IncrementalTopology::lts() const {
		return topology_;
	}

	const std::vector<std::string>& IncrementalTopology::initial_state() const {
		return topology_.initial_state();
	}

	IncrementalTopology::operator const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& () const {
		return topology_;
	}

	const State<std::vector<std::string>, std::pair<size_t, std::string>>& IncrementalTopology::at(const std::vector<std::string>& key) {
		if (topology_.states().contains(key) == true) {
			return topology_.states().at(key);
		} else {
			topology_.AddState(key);
			ExpandState(key);
			return topology_.states().at(key);
		}
	}

	/**
	 * @brief ExpandState is similar to CombineRecursive but it will not recursively expand itself.
	 * We apply the transitions that are found from the given state key based on the current individual resource states.
	 */
	void IncrementalTopology::ExpandState(const std::vector<std::string>& key) {
		PCS_INFO(fmt::format(fmt::fg(fmt::color::plum),
			"[Incremental Topology] Expanding State {}", fmt::join(key, ",")));

		for (size_t i = 0; i < ltss_.size(); ++i) {
			for (const auto& transition : ltss_[i].states().at(key[i]).transitions_) {
				if ((transition.label().find("in:") != std::string::npos) || (transition.label().find("out:") != std::string::npos)) {
					std::optional<std::vector<std::string>> transfer_state = MatchingTransfer(ltss_, key, i, transition);
					if (!transfer_state.has_value()) {
						continue;
					}
					topology_.AddTransition(key, std::make_pair(i, transition.label()), *transfer_state, false);
				} else {
					std::vector<std::string> next_states = key;
					next_states[i] = transition.to();
					topology_.AddTransition(key, std::make_pair(i, transition.label()), next_states, false);
				}
			}
		}
	}

}