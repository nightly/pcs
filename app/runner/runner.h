#pragma once

#include <array>
#include <filesystem>

#include "lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/environment/environment.h"

enum SolverOpt {
	DepthFirst,
	BestTransitions,
	BestResources,
	BestCost,
	BestCostEstimate,
	LocalBestResources,
	LocalBestCost,
	// Multi-Objective
};

struct RunnerOpts {
	SolverOpt solver;
	bool incremental_topology;
	bool generate_images;
	bool only_highlighted_topology_image; // Exports the highlighted topology only, rather than topology & highlighted topology
	bool skeleton_topology_image;
	std::string recipe_name;
};

void Run(const std::string& name, const RunnerOpts& opts);

static size_t NumOfResources(const std::filesystem::path& data_folder);

static pcs::Environment LoadMachine(const std::string& data_folder, size_t num_resources);
static pcs::Recipe LoadRecipe(const std::string& data_folder, const std::string& recipe_file);
static void CompleteTopology(pcs::Environment& machine);
static void IncrementalTopology(pcs::Environment& machine);
static void GraphVizSave(const std::string& export_folder, size_t num_resources, bool only_highlighted_topology, bool skeleton_topology);
static void ControllerGraphVizSave();