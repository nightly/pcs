#include "runner.h"

#include <vector>
#include <optional>
#include <filesystem>
#include <algorithm>
#include <cctype>

#include "lts/parsers/parsers.h"
#include "lts/export.h"
#include "pcs/environment/environment.h"
#include "pcs/environment/writers.h"
#include "pcs/product/recipe.h"
#include "pcs/controller/controller.h"
#include "pcs/controller/highlighter.h"
#include "pcs/common/log.h"

static size_t NumOfResources(const std::filesystem::path& data_folder) {
	size_t count = 0;
	for (const auto& file : std::filesystem::directory_iterator(data_folder)) {
		std::string name = file.path().filename().string();
		std::transform(name.begin(), name.end(), name.begin(),
			[](unsigned char c) { return std::tolower(c); });
		if (name.find("resource") != std::string::npos) {
			count++;
		}
	}
	return count;
}

static pcs::Recipe LoadRecipe(const std::string& data_folder) {
	pcs::Recipe recipe;
	try {
		recipe.set_recipe(data_folder + "recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	return recipe;
}

static pcs::Environment LoadMachine(const std::string& data_folder, size_t num_resources) {
	pcs::Environment machine;
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

static void CompleteTopology(pcs::Environment& machine) {
	machine.Complete();
}

static void IncrementalTopology(pcs::Environment& machine) {
	machine.Incremental();
}

static void GraphVizSave(const std::string& export_folder, size_t num_resources, bool only_highlighted_topology) {
	std::filesystem::current_path(export_folder);
	system("dot -Tpng recipe.gv -o recipe.png");
	for (size_t i = 1; i <= num_resources; ++i) {
		std::string resource = "Resource" + std::to_string(i);
		std::string command = "dot -Tpng " + resource + ".gv -o " + resource + ".png";
		system(command.c_str());
	}
	system("dot -Tpng controller.gv -o controller.png");
	system("dot -Tpng highlighted_topology.gv -o highlighted_topology.png");
	if (!only_highlighted_topology) {
		system("dot -Tpng topology.gv -o topology.png");
	}
}

using ControllerType = nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
	                            boost::hash<std::pair<std::string, std::vector<std::string>>>>;

/*
 * @param name: the name of the example folder to find from `data/` e.g. "hinge" for data/hinge
 * 
 * Expected forms within the folder: ResourceX.txt, recipe.json. 
 * All resources within directory will be used as long as they are sequentially named (Resource1, Resource2, Resource3...).
 */
void Run(const std::string& name, const RunnerOpts& opts) {

	/* Determine number of resources and set data/export folder paths */
	PCS_INFO(fmt::format(fmt::fg(fmt::color::white_smoke), "Using {} Example", name));
	std::string data_folder = "../../data/" + name + '/';

	std::string type;
	switch (opts.solver) {
	case SolverOpt::DepthFirst:
		type = "/dfs/";
		break;
	case SolverOpt::BestFirstTransitions:
		type = "/best-transitions/";
		break;
	case SolverOpt::BestFirstResources:
		type = "/best-resources/";
		break;
	}

	std::string export_folder = "../../exports/" + name + type +
		                        (opts.incremental_topology ? "/incremental/" : "/complete/");
	size_t num_resources = NumOfResources(data_folder);

	/* Load everything */
	pcs::Recipe recipe = LoadRecipe(data_folder);
	nightly::ExportToFile(recipe.lts(), export_folder + "/recipe.gv");

	pcs::Environment machine = LoadMachine(data_folder, num_resources);
	if (opts.incremental_topology) {
		IncrementalTopology(machine);
	} else {
		CompleteTopology(machine);
	}


	std::optional<ControllerType> controller_lts;

	if (opts.solver == SolverOpt::DepthFirst) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		controller_lts = con.Generate();
	} else if (opts.solver == SolverOpt::BestFirstTransitions) {
		pcs::BestController con(&machine, machine.topology(), &recipe);
		controller_lts = con.Generate(pcs::MinimizeOpt::Transitions);
	} else if (opts.solver == SolverOpt::BestFirstResources) {
		pcs::BestController con(&machine, machine.topology(), &recipe);
		controller_lts = con.Generate(pcs::MinimizeOpt::Resources);
	}

	if (controller_lts.has_value()) {
		nightly::ExportToFile(*controller_lts, export_folder + "/controller.gv");
		pcs::Highlighter::HighlightTopology(machine.topology()->lts(), *controller_lts, export_folder + "/highlighted_topology.gv");
	} else {
		PCS_WARN("[PAD] No controller generated");
	}

	/* Export machine, print incremenetal topology stats, and generate images */
	pcs::ExportEnvironment(machine, export_folder);
	PCS_INFO(fmt::format(fmt::fg(fmt::color::white_smoke), "[Topology] Number Of States = {}, Number of Transitions = {}", machine.topology()->lts().NumOfStates(),
		machine.topology()->lts().NumOfTransitions()));
	if (opts.generate_images) {
		GraphVizSave(export_folder, num_resources, opts.only_highlighted_topology_image);
	}
}