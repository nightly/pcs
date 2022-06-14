#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <vector>

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/parsers.h"

static void HingeController2R(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe{ "../../data/hinge/experiments/recipe2R.json"};

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeController2R)->Unit(benchmark::kMillisecond);

static void HingeController3R(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource2.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe{"../../data/hinge/experiments/recipe3R.json"};

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeController3R)->Unit(benchmark::kMillisecond);

static void HingeController4R(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource2.txt", false);
		machine.AddResource("../../data/hinge/Resource3.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe{ "../../data/hinge/experiments/recipe4R.json"};

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeController4R)->Unit(benchmark::kMillisecond);


// 6+ Resources - use normal recipe. Duplicates the testing station resource (R3).
static void HingeController(benchmark::State& state) {
	pcs::Environment machine;
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

	for (size_t i = 5; i < state.range(0); ++i) {
		try {
			machine.AddResource("../../data/hinge/Resource3.txt", false);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
	}
	pcs::Recipe recipe{"../../data/hinge/recipe.json"};
	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeController)->DenseRange(6, 10, 1)->Unit(benchmark::kMillisecond);