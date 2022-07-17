#include "pcs/topology/complete.h"

#include <string>
#include <vector>
#include <span>
#include <set>
#include <utility>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

#include "lts/lts.h"
#include "pcs/topology/topology.h"
#include "pcs/topology/core.h"
#include "lts/state.h"
#include "pcs/operation/parsers/label.h"
#include "pcs/common/log.h"

namespace pcs {

	/*
	 * @param ltss: the LTSs to merge
	 * @param recursive: @default = true. Iterative or recursive DFS.
	 */
	CompleteTopology::CompleteTopology(const std::vector<nightly::LTS<std::string, std::string>>& ltss, bool recursive)
		: ltss_(ltss) {
		std::vector<std::string> initial_key;
		initial_key.reserve(ltss.size());
		for (const auto& lts : ltss) {
			initial_key.emplace_back(lts.initial_state());
		}
		topology_.set_initial_state(initial_key);
		if (recursive) {
			CombineRecursive(initial_key);
		} else {
			CombineRecursive(initial_key); // ...
		}
	}

	const nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& CompleteTopology::lts() const {
		return topology_;
	}

	CompleteTopology::operator const nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& () const {
		return topology_;
	}

	const std::vector<std::string>& CompleteTopology::initial_state() const {
		return topology_.initial_state();
	}

	const nightly::State<std::vector<std::string>, std::pair<size_t, std::string>>& CompleteTopology::at(const std::vector<std::string>& key) {
		return topology_.states().at(key);
	}

	void CompleteTopology::CombineRecursive(std::vector<std::string>& states_vec) {
		if (visited_.contains(states_vec) == true) {
			return;
		}
		visited_.insert(states_vec);

		for (size_t i = 0; i < ltss_.size(); ++i) {
			for (const auto& transition : ltss_[i].states().at(states_vec[i]).transitions_) {
				if ((transition.label().find("in:") != std::string::npos) || (transition.label().find("out:") != std::string::npos)) {
					std::optional<std::vector<std::string>> transfer_state = MatchingTransfer(ltss_, states_vec, i, transition);
					if (!transfer_state.has_value()) {
						continue;
					}
					topology_.AddTransition(states_vec, std::make_pair(i, transition.label()), *transfer_state);
					CombineRecursive(*transfer_state);
				}
				else {
					std::vector<std::string> next_states = states_vec;
					next_states[i] = transition.to();
					topology_.AddTransition(states_vec, std::make_pair(i, transition.label()), next_states);
					CombineRecursive(next_states);
				}
			}
		}
	}

}