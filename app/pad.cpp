#include "pad.h"

#include "pcs/lts/parsers.h"
#include "pcs/machine/machine.h"
#include "pcs/product/recipe.h"

bool LoadPadMachine(pcs::Machine& machine) {
	try {
		machine.AddResource("../../data/pad/R1.txt", false);
		machine.AddResource("../../data/pad/R2.txt", false);
		machine.AddResource("../../data/pad/R3.txt", false);
		machine.AddResource("../../data/pad/R4.txt", false);
		machine.AddResource("../../data/pad/R5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
}

bool LoadPadRecipe(pcs::Recipe& recipe) {
	try {
		recipe.SetRecipe("../../data/pad/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
}