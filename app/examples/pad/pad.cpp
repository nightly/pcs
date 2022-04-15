#include "pad.h"

#include <vector>
#include <optional>

#include "pcs/lts/parsers/string_string.h"
#include "pcs/lts/writers.h"
#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"
#include "pcs/controller/highlighter.h"
#include "pcs/common/log.h"

void PadExample() {
	pcs::Recipe recipe = LoadPadRecipe();
	pcs::ExportToFile(recipe.lts(), "../../exports/pad/recipe.txt");

	pcs::Machine machine = LoadPadMachine();
	const std::vector<pcs::LTS<std::string>>& resources =  machine.resources();
	for (size_t i = 0; i < machine.NumOfResources(); i++) {
		std::string path = "../../exports/pad/Resource" + std::to_string(i + 1) + ".txt";
		pcs::ExportToFile(resources[i], path);
	}

	ComputePadTopology(machine);
	const pcs::LTS<std::string>& topology = machine.topology();
	pcs::ExportToFile(topology, "../../exports/pad/topology.txt");
	
	std::optional<pcs::LTS<std::string>> controller = pcs::GenerateController(machine, recipe);
	if (controller.has_value()) {
		pcs::ExportToFile(*controller, "../../exports/pad/controller.txt");
		pcs::HighlightTopology(topology, *controller, "../../exports/pad/highlighted_topology.txt");
	} else {
		PCS_INFO("[PAD] No controller generated");
	}
}

pcs::Recipe LoadPadRecipe() {
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_s.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return recipe;
}

pcs::Machine LoadPadMachine() {
	pcs::Machine machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return machine;
}

void ComputePadTopology(pcs::Machine& machine) {
	machine.ComputeTopology();
}
