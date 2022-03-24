#include "pcs/product/recipe.h"

#include "pcs/lts/parsers.h"

namespace pcs {

	Recipe::Recipe() {};

	/*
	 * @brief Recipe constructor overload - read from JSON file, defers to LTS template specialization
	 * @param filepath: JSON filepath of the recipe to parse into a LTS<TransitionT = CompositeOperation>
	 */
	Recipe::Recipe(const std::filesystem::path& filepath) 
		: lts_(filepath) {
	}

}