#include "pad.h"

#include <vector>
#include <optional>

#include "pcs/lts/parsers/string_string.h"
#include "pcs/lts/writers.h"
#include "pcs/lts/export.h"
#include "pcs/machine/machine.h"
#include "pcs/machine/writers.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"
#include "pcs/controller/highlighter.h"
#include "pcs/common/log.h"

using namespace pcs;

void PadExample() {
	pcs::Recipe recipe = LoadPadRecipe();
	pcs::ExportToFile(recipe.lts(), "../../exports/pad/recipe.txt");

	pcs::Machine machine = LoadPadMachine();
	ComputePadTopology(machine);
	pcs::ExportMachine(machine, "../../exports/pad");
	
	Controller con(&machine, &recipe);
	std::optional<const LTS<std::string, std::string>*> controller_lts = con.Generate();
	if (controller_lts.has_value()) {
		pcs::ExportToFile(**controller_lts, "../../exports/pad/controller.txt");
		pcs::HighlightTopology(machine.topology(), **controller_lts, "../../exports/pad/highlighted_topology.txt");
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
