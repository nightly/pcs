#pragma once

#include <vector>
#include <unordered_map>

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {

	using namespace pcs::lts;

	class Recipe {
	public:
		LabelledTransitionSystem<CompositeOperation> lts_;
	public:
		Recipe();
		Recipe(const std::filesystem::path& filepath);
	};
}
