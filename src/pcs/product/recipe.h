#pragma once

#include <vector>
#include <unordered_map>

#include "lts/lts.h"
#include "pcs/operation/composite.h"

namespace pcs {

	class Recipe {
	private:
		nightly::LTS<std::string, CompositeOperation> lts_;
	public:
		Recipe() = default;
		Recipe(const std::filesystem::path& filepath);
		~Recipe() = default;

		const nightly::LTS<std::string, CompositeOperation>& lts() const;
		void set_recipe(const std::filesystem::path& filepath);

		operator const nightly::LTS<std::string, CompositeOperation>& () const;

	};
}
