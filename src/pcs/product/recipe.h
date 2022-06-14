#pragma once

#include <vector>
#include <unordered_map>

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {

	class Recipe {
	private:
		LTS<std::string, CompositeOperation> lts_;
	public:
		Recipe() = default;
		Recipe(const std::filesystem::path& filepath);
		~Recipe() = default;

		const LTS<std::string, CompositeOperation>& lts() const;
		void set_recipe(const std::filesystem::path& filepath);

		operator const LTS<std::string, CompositeOperation>& () const;

	};
}
