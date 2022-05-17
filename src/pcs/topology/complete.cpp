#include "pcs/topology/complete.h"

#include <string>
#include <vector>
#include <span>
#include <set>
#include <utility>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/topology/core.h"
#include "pcs/lts/state.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/common/log.h"

namespace pcs {

	CompleteTopology::CompleteTopology(const std::span<LTS<std::string, std::string>>& ltss)
	 :ltss_(ltss) {
		std::vector<std::string> initial_key;
		initial_key.reserve(ltss.size());
		for (const auto& lts : ltss) {
			initial_key.emplace_back(lts.initial_state());
		}
		topology_.set_initial_state(initial_key);
		CombineRecursive(initial_key);
	}

	const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& CompleteTopology::lts() const {
		return topology_;
	}

	const State<std::vector<std::string>, std::pair<size_t, std::string>>& CompleteTopology::at(const std::vector<std::string>& key) {
		return topology_.states().at(key);
	}

	const std::vector<std::string>& CompleteTopology::initial_state() const {
		return topology_.initial_state();
	}

	void CompleteTopology::CombineRecursive(std::vector<std::string>& states_vec) {
		if (visited_.contains(states_vec) == true) {
			return;
		}
		visited_.insert(states_vec);

		for (size_t i = 0; i < ltss_.size(); ++i) {
			for (const auto& transition : ltss_[i].states().at(states_vec[i]).transitions_) {
				if ((transition.first.find("in:") != std::string::npos) || (transition.first.find("out:") != std::string::npos)) {
					std::optional<std::vector<std::string>> transfer_state = MatchingTransfer(ltss_, states_vec, i, transition);
					if (!transfer_state.has_value()) {
						continue;
					}
					topology_.AddTransition(states_vec, std::make_pair(i, transition.first), *transfer_state);
					CombineRecursive(*transfer_state);
				}
				else {
					std::vector<std::string> next_states = states_vec;
					next_states[i] = transition.second;
					topology_.AddTransition(states_vec, std::make_pair(i, transition.first), next_states);
					CombineRecursive(next_states);
				}
			}
		}
	}

}