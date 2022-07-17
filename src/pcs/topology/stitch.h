#pragma once

#include "lts/lts.h"
#include <span>

namespace pcs {

	void RemoveResources(nightly::LTS<>& topology, std::initializer_list<size_t> resources);
	void AddResources(nightly::LTS<>& topology, const std::span<nightly::LTS<>>& ltss);
}