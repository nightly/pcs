#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <array>

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/string_string.h"

static void BM_HingeControllerCompleteTopology(benchmark::State& state) {
	pcs::System machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource2.txt", false);
		machine.AddResource("../../data/hinge/Resource3.txt", false);
		machine.AddResource("../../data/hinge/Resource4.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	}
	catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.Complete();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/hinge/recipe.json");
	}
	catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Controller con(&machine, machine.topology(), &recipe);
	for (auto _ : state) {
		std::optional<const pcs::LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_HingeControllerCompleteTopology);


static void BM_HingeControllerIncrementalTopology(benchmark::State& state) {
	pcs::System machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource2.txt", false);
		machine.AddResource("../../data/hinge/Resource3.txt", false);
		machine.AddResource("../../data/hinge/Resource4.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	}
	catch (const std::ifstream::failure& e) {
		throw;
	}
	machine.Incremental();
	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe.json");
	}
	catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Controller con(&machine, machine.topology(), &recipe);
	for (auto _ : state) {
		std::optional<const pcs::LTS<std::vector<std::string>, std::string, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_HingeControllerIncrementalTopology);

