#include "pad.h"

#include <vector>

#include "pcs/lts/parsers.h"
#include "pcs/lts/writers.h"
#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"

void PadExample() {
	pcs::Recipe recipe = LoadPadRecipe();
	pcs::ExportToFile(recipe.lts_, "../../exports/pad/recipe.txt");

	pcs::Machine machine = LoadPadMachine();
	const std::vector<pcs::LabelledTransitionSystem<std::string>>& resources =  machine.GetResources();
	for (size_t i = 0; i < machine.NumberOfResources(); i++) {
		std::string path = "../../exports/pad/Resource" + std::to_string(i + 1) + ".txt";
		pcs::ExportToFile(resources[i], path);
	}

	ComputePadTopology(machine);
	const pcs::LabelledTransitionSystem<std::string>& topology = machine.GetTopology();
	pcs::ExportToFile(topology, "../../exports/pad/topology.txt");
	
	pcs::LabelledTransitionSystem<std::string> controller = pcs::GenerateController(machine, recipe);
	pcs::ExportToFile(controller, "../../exports/pad/controller.txt");
}

pcs::Recipe LoadPadRecipe() {
	pcs::Recipe recipe;
	try {
		recipe.SetRecipe("../../data/pad/recipe_s.json");
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
