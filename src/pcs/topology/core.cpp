#include "pcs/topology/core.h"

#include <vector>
#include <span>
#include <string>
#include <utility>
#include <optional>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/lts.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {


	/*
	 * @brief When coming across an "in:X" or "out:X" transition, a corresponding inverse is required.
	 * @return The end-state of applying the two transitions if found.
	 */
	std::optional<std::vector<std::string>> MatchingTransfer(const std::vector<LTS<std::string, std::string>>& ltss, const std::vector<std::string>& states_vec,
		                                    size_t current_ltss_idx, const Transition<std::string, std::string>& current_transition) {
		TransferOperation transfer = *(StringToTransfer(current_transition.label()));
		TransferOperation inverse = transfer.Inverse();

		for (size_t i = 0; i < ltss.size(); ++i) {
			if (i == current_ltss_idx) {
				continue;
			}
			for (const auto& t : ltss[i].states().at(states_vec[i]).transitions_) {
				if (t.label().find(inverse.name()) != std::string::npos) {
					std::vector<std::string> resulting_state = states_vec;
					resulting_state[current_ltss_idx] = current_transition.to();
					resulting_state[i] = t.to();
					return resulting_state;
				}
			}
		}
		return {};
	}

}