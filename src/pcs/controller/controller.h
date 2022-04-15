#pragma once

#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"

#include <optional>

namespace pcs {

	class ControllerGenerator {
	private:
		LTS<std::string, std::string> controller_;
		const Machine& machine_;
		const LTS<std::string, std::string>& topology_;
		const Recipe& recipe_;
		std::vector<std::vector<std::string>> parts_;
		std::string recipe_state_;
		std::string topology_state_;
	public:
		ControllerGenerator(const Machine& machine, const Recipe& recipe);
		std::optional<const LTS<std::string, std::string>*> Generate();
	private:
		bool HandleComposite(const CompositeOperation& co);
		bool ProcessRecipe(const std::string& state);

		size_t CurrentResource(const std::string& old_state, const std::string& new_state);
	};
}