#pragma once

#include "pcs/lts/lts.h"
#include <span>

namespace pcs {

	void RemoveResources(LTS<>& topology, std::initializer_list<size_t> resources);
	void AddResources(LTS<>& topology, const std::span<LTS<>>& ltss);
}