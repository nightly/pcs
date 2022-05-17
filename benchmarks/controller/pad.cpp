#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <array>

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/string_string.h"

static void BM_PADControllerCompleteTopology(benchmark::State& state) {
	pcs::System machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.Complete();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_full.json");
	}catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Controller con(&machine, machine.topology(), &recipe);
	for (auto _ : state) {
		std::optional<const pcs::LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_PADControllerCompleteTopology);


static void BM_PADControllerIncrementalTopology(benchmark::State& state) {
	pcs::System machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.Incremental();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe_full.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Controller con(&machine, machine.topology(), &recipe);
	for (auto _ : state) {
		std::optional<const pcs::LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_PADControllerIncrementalTopology);

