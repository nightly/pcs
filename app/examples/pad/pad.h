#pragma once

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/machine/machine.h"

void PadExample();
pcs::Machine LoadPadMachine();
pcs::Recipe LoadPadRecipe();
void ComputePadTopology(pcs::Machine& machine);