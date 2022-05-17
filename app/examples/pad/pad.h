#pragma once

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/system/system.h"

void PadExample(bool incremental);

static pcs::System LoadPadMachine();

static pcs::Recipe LoadPadRecipe();

static void CompletePadTopology(pcs::System& machine);

static void IncrementalPadTopology(pcs::System& machine);