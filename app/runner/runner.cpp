#include "runner.h"

#include <vector>
#include <optional>

#include "pcs/lts/parsers/parsers.h"
#include "pcs/lts/export.h"
#include "pcs/system/system.h"
#include "pcs/system/writers.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"
#include "pcs/controller/highlighter.h"
#include "pcs/common/log.h"

static pcs::Recipe LoadRecipe(const std::string& data_folder) {
	pcs::Recipe recipe;
	try {
		recipe.set_recipe(data_folder + "recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return recipe;
}

static pcs::System LoadMachine(const std::string& data_folder, size_t num_resources) {
	pcs::System machine;
	try {
		std::filesystem::current_path(data_folder);
		for (size_t i = 1; i <= num_resources; ++i) {
			machine.AddResource("Resource" + std::to_string(i) + ".txt", false);
		}
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return machine;
}

static void CompleteTopology(pcs::System& machine) {
	machine.Complete();
	PCS_INFO(fmt::format(fmt::fg(fmt::color::white_smoke), "[Topology] Number Of States = {}, Number of Transitions = {}", machine.topology()->lts().NumOfStates(),
		machine.topology()->lts().NumOfTransitions()));
}

static void IncrementalTopology(pcs::System& machine) {
	machine.Incremental();
}

static void GraphVizSave(const std::string& export_folder, size_t num_resources) {
	std::filesystem::current_path(export_folder);
	system("dot -Tpng recipe.txt -o recipe.png");
	for (size_t i = 1; i <= num_resources; ++i) {
		std::string resource = "Resource" + std::to_string(i);
		std::string command = "dot -Tpng " + resource + ".txt -o " + resource + ".png";
		system(command.c_str());
	}
	system("dot -Tpng controller.txt -o controller.png");
	system("dot -Tpng highlighted_topology.txt -o highlighted_topology.png");
	system("dot -Tpng topology.txt -o topology.png");
}


/*
 * @param name: the name of the example folder to find from `data/` e.g. "hinge" for data/hinge
 * Expected forms within the folder: ResourceX.txt, recipe.json
 */
void Run(const std::string& name, size_t num_resources, bool incremental, bool generate_images) {
	PCS_INFO(fmt::format(fmt::fg(fmt::color::white_smoke), "Using {} Example", name));
	
	std::string data_folder = "../../data/" + name + '/';
	std::string export_folder = "../../exports/" + name + (incremental ? "/incremental/" : "/complete/");

	pcs::Recipe recipe = LoadRecipe(data_folder);
	pcs::ExportToFile(recipe.lts(), export_folder + "/recipe.txt");

	pcs::System machine = LoadMachine(data_folder, num_resources);
	if (incremental) {
		IncrementalTopology(machine);
	} else {
		CompleteTopology(machine);
	}

	pcs::Controller con(&machine, machine.topology(), &recipe);
	auto controller_lts = con.Generate();

	if (controller_lts.has_value()) {
		pcs::ExportToFile(**controller_lts, export_folder + "/controller.txt");
		pcs::HighlightTopology(machine.topology()->lts(), **controller_lts, export_folder + "/highlighted_topology.txt");
	} else {
		PCS_WARN("[PAD] No controller generated");
	}
	pcs::ExportMachine(machine, export_folder);

	if (generate_images) {
		GraphVizSave(export_folder, num_resources);
	}
}