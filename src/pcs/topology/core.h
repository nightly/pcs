#pragma once

#include <vector>
#include <span>
#include <string>
#include <utility>
#include <optional>

#include "pcs/lts/lts.h"
#include "pcs/lts/transition.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {

	std::optional<std::vector<std::string>> MatchingTransfer(const std::vector<LTS<std::string, std::string>>& ltss, const std::vector<std::string>& states_vec,
		size_t current_ltss_idx, const Transition<std::string, std::string>& current_transition);

}