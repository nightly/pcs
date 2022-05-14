#pragma once

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/system/system.h"

void PadExample();

pcs::System LoadPadMachine();

pcs::Recipe LoadPadRecipe();

void ComputePadTopology(pcs::System& machine);