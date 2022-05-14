#pragma once

#include <vector>
#include <optional>
#include <iostream>

#include "pcs/lts/parsers/string_string.h"
#include "pcs/lts/export.h"
#include "pcs/system/system.h"
#include "pcs/system/writers.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"
#include "pcs/common/log.h"

inline static pcs::Recipe LoadExpRecipe() {
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_s.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return recipe;
}

inline static pcs::System LoadExpMachine() {
	pcs::System machine;
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

inline static void ComputeExpTopology(pcs::System& machine) {
	machine.ComputeTopology();
}

inline void Experimental() {
	pcs::Recipe recipe = LoadExpRecipe();
	pcs::ExportToFile(recipe.lts(), "../../exports/experimental/recipe.txt");

	pcs::System machine = LoadExpMachine();
	ComputeExpTopology(machine);
	pcs::ExportMachine(machine, "../../exports/experimental");

	pcs::Controller con(&machine, &recipe);
	std::optional<const pcs::LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> controller = con.Generate();
	if (controller.has_value()) {
		pcs::ExportToFile(**controller, "../../exports/experimental/controller.txt");
		// Highlight
	} else {
		PCS_INFO("[Experimental] No controller generated");
	}
}
