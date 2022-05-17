#pragma once

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/system/system.h"

void HingeExample(bool incremental);

static pcs::System LoadHingeMachine();

static pcs::Recipe LoadHingeRecipe();

static void CompleteHingeTopology(pcs::System& machine);

static void IncrementalHingeTopology(pcs::System& machine);