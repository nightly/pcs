#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <vector>
#include <filesystem>

#include "pcs/topology/topology.h"
#include "lts/parsers/parsers.h"

static const std::string machine_name = "model1";
static const std::string machine_dir = "../../data/" + machine_name;
static const std::string resource_file_prefix = machine_dir + "/Resource";
static const std::string recipe_dir = machine_dir + "/experiments/";

static const int num_resources = 10;

static bool resources_created = true;
static bool recipes_created = true;

static void CreateResource(int i) {
	const std::string filepath = resource_file_prefix + std::to_string(i) + ".txt";
	std::ofstream stream;
	stream.exceptions(std::ofstream::badbit);

	try {
		stream.open(filepath, std::ios::out | std::ios::trunc);

		stream << "s0\n";
		stream << "s0. nop. s0.\n";
		stream << "s1. nop. s1.\n";
		stream << "s2. nop. s2.\n";

		if (i == 1) {
			stream << "s0. o11. s1.\n";
			stream << "s0. o1. s1.\n";
			stream << "s1. out:1. s0.\n";
			stream << "s1. rem. s0.\n";
			stream << "s1. o12. s2.\n";
			stream << "s1. o13. s2.\n";
			stream << "s2. out:1. s0.\n";
			stream << "s2. rem. s0.\n";
		} else {
			stream << "s0. o" + std::to_string(i) + "1. s1.\n";
			stream << "s0. in:" + std::to_string(i) + ". s1.\n";
			stream << "s1. out:" + std::to_string(i) + ". s0.\n";
			stream << "s1. rem. s0.\n";
			stream << "s1. o" + std::to_string(i) + "2. s2.\n";
			stream << "s1. o" + std::to_string(i) + ". s2.\n";
			stream << "s2. out:" + std::to_string(i) + ". s0.\n";
			stream << "s2. rem. s0.\n";
		}
	}
	catch (const std::ofstream::failure& e) {
		throw;
	}
}

static void CreateTransportResource(int t, int n) {
	const std::string filepath = resource_file_prefix + std::to_string(t) + ".txt";
	std::ofstream stream;
	stream.exceptions(std::ofstream::badbit);

	try {
		stream.open(filepath, std::ios::out | std::ios::trunc);
		stream << "s0\n";
		stream << "s0. nop. s0.\n";

		for (int i = 1; i < n; i++) {
			stream << "s0. in:" + std::to_string(i) + ". s" + std::to_string(i) + ".\n";
			stream << "s" + std::to_string(i) + ". out:" + std::to_string(i + 1) + ". s0.\n";
		}
	}
	catch (const std::ofstream::failure& e) {
		throw;
	}
}

static void LoadResources(pcs::Environment& machine, int n, int transport) {
	resources_created = std::filesystem::is_directory(machine_dir);
	if (!resources_created) {
		std::filesystem::create_directory(machine_dir);

		for (int i = 1; i < num_resources; i++) {
			CreateResource(i);
		}

		CreateTransportResource(num_resources, num_resources);
		resources_created = true;
	}

	try {
		for (int i = 1; i < n; i++) {
			machine.AddResource(resource_file_prefix + std::to_string(i) + ".txt", false);
		}

		machine.AddResource(resource_file_prefix + std::to_string(transport) + ".txt", false);
	}
	catch (const std::ifstream::failure& e) {
		throw;
	}
}

static void CreateRecipe(int r) {
	const std::string filepath = recipe_dir + "recipe" + std::to_string(r) + "R.json";
	std::ofstream stream;
	stream.exceptions(std::ofstream::badbit);

	try {
		stream.open(filepath, std::ios::out | std::ios::trunc);
		stream << "{\n";
		stream << "  \"initialState\": \"A1\",\n";
		stream << "  \"transitions\": [\n";

		for (int i = 1; i <= r; i++) {
			stream << "    {\n";

			stream << "      \"startState\": \"A" + std::to_string(i) + "\",\n";
			stream << "      \"label\": {\n";
			stream << "        \"guard\": {},\n";
			stream << "        \"sequential\": [\n";
			stream << "          {\n";
			stream << "            \"name\": \"o" + std::to_string(i) + "\",\n";
			stream << "            \"input\": [";

			if (i > 1) {
				stream << "\"p" + std::to_string(i - 1) + "\"";
			}

			stream << "],\n";			
			stream << "            \"parameters\": [],\n";
			stream << "            \"output\": [\"p" + std::to_string(i) + "\"]\n";
			stream << "          }\n";
			stream << "        ],\n";
			stream << "        \"parallel\": []\n";
			stream << "      },\n";
			stream << "      \"endState\": \"A" + std::to_string(i + 1) + "\"\n";
			stream << "    }";

			if (i < r) {
				stream << ",";
			}

			stream << "\n";
		}

		stream << "  ]\n";
		stream << "}\n";
	}
	catch (const std::ofstream::failure& e) {
		throw;
	}
}

static void LoadRecipe(pcs::Recipe& recipe, int n) {
	recipes_created = std::filesystem::is_directory(recipe_dir);
	if (!recipes_created) {
		std::filesystem::create_directory(recipe_dir);

		for (int i = 1; i <= num_resources - 1; i++) {
			CreateRecipe(i);
		}

		recipes_created = true;
	}

	recipe.set_recipe(recipe_dir + "recipe" + std::to_string(n - 1) + "R.json");
}

static void CompleteTopology(benchmark::State& state, int n, int transport) {
	pcs::Environment machine;
	LoadResources(machine, n, transport);

	for (auto _ : state) {
		machine.Complete();
		benchmark::DoNotOptimize(machine.topology());
		benchmark::ClobberMemory();
	}
}

static void ControllerUsingCompleteTopology(benchmark::State& state, int n, int transport) {
	pcs::Environment machine;
	LoadResources(machine, n, transport);
	pcs::Recipe recipe;
	LoadRecipe(recipe, n);

	machine.Complete();

	for (auto _ : state) {
		pcs::Controller con(&machine, machine.topology(), &recipe);
		auto controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

static void CompleteTopologyWithController(benchmark::State& state, int n, int transport) {
	pcs::Environment machine;
	LoadResources(machine, n, transport);
	pcs::Recipe recipe;
	LoadRecipe(recipe, n);

	for (auto _ : state) {
		machine.Complete();
		pcs::Controller con(&machine, machine.topology(), &recipe);
		auto controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

static void IncrementalTopologyWithController(benchmark::State& state, int n, int transport) {
	pcs::Environment machine;
	LoadResources(machine, n, transport);
	pcs::Recipe recipe;
	LoadRecipe(recipe, n);

	for (auto _ : state) {
		machine.Incremental();
		pcs::Controller con(&machine, machine.topology(), &recipe);
		auto controller_lts = con.Generate();
		benchmark::DoNotOptimize(controller_lts);
		benchmark::ClobberMemory();
	}
}

static void CompleteTopology(benchmark::State& state) {
	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopology(state, i, num_resources);
	}
}

BENCHMARK(CompleteTopology)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void ControllerUsingCompleteTopology(benchmark::State& state) {
	for (size_t i = 2; i <= state.range(0); ++i) {
		ControllerUsingCompleteTopology(state, i, num_resources);
	}
}

BENCHMARK(ControllerUsingCompleteTopology)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void CompleteTopologyWithController(benchmark::State& state) {
	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopologyWithController(state, i, num_resources);
	}
}

BENCHMARK(CompleteTopologyWithController)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void IncrementalTopologyWithController(benchmark::State& state) {
	for (size_t i = 2; i <= state.range(0); ++i) {
		IncrementalTopologyWithController(state, i, num_resources);
	}
}

BENCHMARK(IncrementalTopologyWithController)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);