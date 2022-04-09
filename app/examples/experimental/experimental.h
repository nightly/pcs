#pragma once

#include <vector>
#include <optional>
#include <iostream>

#include "pcs/lts/parsers.h"
#include "pcs/lts/writers.h"
#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"

inline static pcs::Recipe LoadExpRecipe() {
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_s.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return recipe;
}

inline static pcs::Machine LoadExpMachine() {
	pcs::Machine machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		//machine.AddResource("../../data/pad/Resource2.txt", false);
		//machine.AddResource("../../data/pad/Resource3.txt", false);
		//machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return machine;
}

inline static void ComputeExpTopology(pcs::Machine& machine) {
	machine.ComputeTopology();
}

inline void Experimental() {
	pcs::Recipe recipe = LoadExpRecipe();
	pcs::ExportToFile(recipe.lts_, "../../exports/experimental/recipe.txt");

	pcs::Machine machine = LoadExpMachine();
	const std::vector<pcs::LabelledTransitionSystem<std::string>>& resources = machine.resources();
	for (size_t i = 0; i < machine.NumOfResources(); i++) {
		std::string path = "../../exports/experimental/Resource" + std::to_string(i + 1) + ".txt";
		pcs::ExportToFile(resources[i], path);
	}

	ComputeExpTopology(machine);
	const pcs::LabelledTransitionSystem<std::string>& topology = machine.topology();
	pcs::ExportToFile(topology, "../../exports/experimental/topology.txt");

	std::optional<pcs::LabelledTransitionSystem<std::string>> controller = pcs::GenerateController(machine, recipe);
	if (controller.has_value()) {
		pcs::ExportToFile(*controller, "../../exports/experimental/controller.txt");
	} else {
		std::cout << "[Experimental] No controller generated\n";
	}
}
