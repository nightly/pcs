#pragma once

#include <vector>
#include <unordered_map>

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {

	class Recipe {
	public:
		LabelledTransitionSystem<std::string, CompositeOperation> lts_;
	public:
		Recipe() = default;
		Recipe(const std::filesystem::path& filepath);
		void set_recipe(const std::filesystem::path& filepath);
	};
}
