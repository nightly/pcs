#pragma once

#include <string>
#include <span>
#include <vector>

#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs::lts {

	LabelledTransitionSystem Combine(const std::span<LabelledTransitionSystem>& ltss);
	void CombineRecursive(const std::span<LabelledTransitionSystem>& ltss, std::vector<std::string>& states_vec,
		LabelledTransitionSystem& combined_lts);
	std::string StateVectorToString(const std::span<std::string>& vec);
}