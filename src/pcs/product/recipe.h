#pragma once

#include <vector>
#include <unordered_map>

#include "pcs/lts/lts.h"

namespace pcs {

	using LabelledTransitionSystem = pcs::lts::LabelledTransitionSystem;
	using State = pcs::lts::State;

	class Recipe {
	public:
		LabelledTransitionSystem lts_;
	public:
		Recipe();
		Recipe(const std::filesystem::path& filepath);
	};
}
