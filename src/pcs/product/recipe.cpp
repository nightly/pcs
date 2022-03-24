#include "pcs/product/recipe.h"

#include "pcs/lts/parsers.h"

namespace pcs {

	Recipe::Recipe() {};

	/*
	 * @brief Recipe constructor overload - read from file
	 * @param filepath: filepath of the LTS to parse into a recipe
	 */
	Recipe::Recipe(const std::filesystem::path& filepath) 
		: lts_(filepath) {
	}

}