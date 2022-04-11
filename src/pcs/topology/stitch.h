#pragma once

#include "pcs/lts/lts.h"
#include <span>

namespace pcs {

	void RemoveResources(LabelledTransitionSystem<>& topology, std::initializer_list<size_t> resources);
	void AddResources(LabelledTransitionSystem<>& topology, const std::span<LabelledTransitionSystem<>>& ltss);
}