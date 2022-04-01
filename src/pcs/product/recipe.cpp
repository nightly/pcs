#include "pcs/product/recipe.h"

#include "pcs/product/parser.h"

namespace pcs {

	/*
	 * @brief Recipe constructor overload - read from JSON file, defers to LTS template specialization
	 * @param filepath: JSON filepath of the recipe to parse into a LTS<TransitionT = CompositeOperation>
	 */
	Recipe::Recipe(const std::filesystem::path& filepath)  {
		SetRecipe(filepath);
	}

	void Recipe::SetRecipe(const std::filesystem::path& filepath) {
		try {
			ReadFromJsonFile(lts_, filepath);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	}

}