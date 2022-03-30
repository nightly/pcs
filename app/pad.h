#pragma once

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/machine/machine.h"


bool LoadPadMachine(pcs::Machine& machine);
bool LoadPadRecipe(pcs::Recipe& recipe);