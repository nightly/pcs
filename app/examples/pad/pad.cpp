#include "pad.h"

#include <vector>
#include <optional>

#include "pcs/lts/parsers/string_string.h"
#include "pcs/lts/writers.h"
#include "pcs/lts/export.h"
#include "pcs/system/system.h"
#include "pcs/system/writers.h"
#include "pcs/product/recipe.h"
 #include "pcs/controller/controller.h"
#include "pcs/controller/highlighter.h"
#include "pcs/common/log.h"

using namespace pcs;

void PadExample(bool incremental) {
	pcs::Recipe recipe = LoadPadRecipe();
	pcs::ExportToFile(recipe.lts(), "../../exports/pad/recipe.txt");

	pcs::System machine = LoadPadMachine();
	if (incremental) {
		IncrementalPadTopology(machine);
	} else {
		CompletePadTopology(machine);
	}
	pcs::ExportMachine(machine, "../../exports/pad");
		
	Controller con(&machine, machine.topology(), &recipe);
	
	std::optional<const LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
	if (controller_lts.has_value()) {
		pcs::ExportToFile(**controller_lts, "../../exports/pad/controller.txt");
		pcs::HighlightTopology(machine.topology()->lts(), **controller_lts, "../../exports/pad/highlighted_topology.txt");
	} else {
		PCS_WARN("[PAD] No controller generated");
	}
}

static pcs::Recipe LoadPadRecipe() {
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_full.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return recipe;
}

static pcs::System LoadPadMachine() {
	pcs::System machine;
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

static void CompletePadTopology(pcs::System& machine) {
	machine.Complete();
	PCS_INFO("[Topology] Number Of States = {}, Number of Transitions = {}", machine.topology()->lts().NumOfStates(), 
		machine.topology()->lts().NumOfTransitions());
}

static void IncrementalPadTopology(pcs::System& machine) {
	machine.Incremental();
}