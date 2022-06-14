#include "pcs/product/recipe.h"

#include "pcs/product/parsers/recipe.h"

namespace pcs {

	/*
	 * @brief Recipe constructor overload - read from JSON file, defers to LTS template specialization
	 * @param filepath: JSON filepath of the recipe to parse into a LTS<KeyT = String, TransitionT = CompositeOperation>
	 * @exception Propagates std::ifstream::failure
	 */
	Recipe::Recipe(const std::filesystem::path& filepath)  {
		try {
			set_recipe(filepath);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	const LTS<std::string, CompositeOperation>& Recipe::lts() const {
		return lts_;
	}

	/*
	 * @brief Parses a recipe JSON file into a LTS
	 * @exception Propagates std::ifstream::failure
	 */
	void Recipe::set_recipe(const std::filesystem::path& filepath) {
		try {
			ReadFromJsonFile(lts_, filepath);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	Recipe::operator const LTS<std::string, CompositeOperation>& () const {
		return lts_;
	}

}