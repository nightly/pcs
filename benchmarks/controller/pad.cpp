#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <array>

#include "pcs/topology/topology.h"
#include "../memory_manager.h"

static void BM_PADControllerCompleteTopology(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	for (auto _ : state) {
		machine.Complete();
		pcs::Controller con(&machine, machine.topology(), &recipe);
		auto controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_PADControllerCompleteTopology)->Unit(benchmark::kMillisecond);


static void BM_PADControllerIncrementalTopology(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/pad/Resource1.txt", false);
		machine.AddResource("../../data/pad/Resource2.txt", false);
		machine.AddResource("../../data/pad/Resource3.txt", false);
		machine.AddResource("../../data/pad/Resource4.txt", false);
		machine.AddResource("../../data/pad/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/pad/recipe.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	for (auto _ : state) {
		machine.Incremental();
		pcs::Controller con(&machine, machine.topology(), &recipe);
		auto controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_PADControllerIncrementalTopology)->Unit(benchmark::kMillisecond);

//BENCHMARK_MAIN();
int main(int argc, char** argv)
{
	::benchmark::RegisterMemoryManager(mm.get());
	::benchmark::Initialize(&argc, argv);
	::benchmark::RunSpecifiedBenchmarks();
	::benchmark::RegisterMemoryManager(nullptr);
}
