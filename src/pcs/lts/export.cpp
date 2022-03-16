#include "pcs/lts/export.h"

#include <string>
#include <ostream>

#include "pcs/lts/lts.h"

namespace pcs::lts {

	/*
	 * @brief Output operator overload
	 */
	std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem& lts) {
		if (lts.initial_state_.empty() && lts.states_.empty()) {
			os << "Empty Labelled Transition System\n";
			return os;
		}
		os << "Initial state: " << lts.initial_state_ << '\n';
		for (const auto& pair : lts.states_) {
			os << pair.second;
		}
		return os;
	}

}