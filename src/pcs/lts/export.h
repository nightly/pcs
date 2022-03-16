#pragma once

#include <string>
#include <ostream>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"

namespace pcs::lts {
	std::ostream& operator<<(std::ostream& os, const LabelledTransitionSystem& lts);
}