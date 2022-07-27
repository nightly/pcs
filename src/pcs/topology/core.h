#pragma once

#include <vector>
#include <span>
#include <string>
#include <utility>
#include <optional>

#include "lts/lts.h"
#include "lts/transition.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {

	std::optional<std::vector<std::string>> MatchingTransfer(const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss,
		                                                    const std::vector<std::string>& states_vec, size_t current_ltss_idx, 
		                                                    const nightly::Transition<ParameterizedOp, std::string>& current_transition);

	bool NopizeObservable(const std::vector<std::string>& states_vec, size_t resource);
	bool NopizeSync(const std::vector<std::string>& states_vec, size_t in, size_t out);

}