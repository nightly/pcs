#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <array>

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/string_string.h"

static void BM_PadController(benchmark::State& state) {
	pcs::Machine machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.ComputeTopology();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_s.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

    for (auto _ : state) {
		pcs::Controller con(&machine, &recipe);
		std::optional<const pcs::LTS<std::string, std::string>*> controller_lts = con.Generate();
    }
}

BENCHMARK(BM_PadController);