#pragma once

#include <array>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/system/system.h"

void Run(const std::string& name, bool incremental, bool generate_images, bool only_highlighted_topology=true);

static size_t NumOfResources(const std::filesystem::path& data_folder);

static pcs::System LoadMachine(const std::string& data_folder, size_t num_resources);
static pcs::Recipe LoadRecipe(const std::string& data_folder);
static void CompleteTopology(pcs::System& machine);
static void IncrementalTopology(pcs::System& machine);
static void GraphVizSave(const std::string& export_folder, size_t num_resources, bool only_highlighted_topology);