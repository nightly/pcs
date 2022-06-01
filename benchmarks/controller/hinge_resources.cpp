#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <vector>

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/parsers.h"

static void HingeComplete2ResourceTopology(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/hinge/experiments/recipe2R.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeComplete2ResourceTopology)->Unit(benchmark::kMillisecond);

static void HingeComplete3ResourceTopology(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource2.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/hinge/experiments/recipe3R.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeComplete3ResourceTopology)->Unit(benchmark::kMillisecond);

static void HingeComplete4ResourceTopology(benchmark::State& state) {
	pcs::Environment machine;
	try {
		machine.AddResource("../../data/hinge/Resource1.txt", false);
		machine.AddResource("../../data/hinge/Resource2.txt", false);
		machine.AddResource("../../data/hinge/Resource3.txt", false);
		machine.AddResource("../../data/hinge/Resource5.txt", false);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	pcs::Recipe recipe;
	try {
		recipe.set_recipe("../../data/hinge/experiments/recipe4R.json");
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		std::optional<const pcs::LTS<std::vector<std::string>, std::vector<std::string>, boost::hash<std::vector<std::string>>>*> controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(HingeComplete4ResourceTopology)->Unit(benchmark::kMillisecond);
