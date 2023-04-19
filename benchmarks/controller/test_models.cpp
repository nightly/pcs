#include <benchmark/benchmark.h>
#include "pcs/controller/controller.h"

#include <vector>
#include <filesystem>

#include "pcs/topology/topology.h"
#include "lts/parsers/parsers.h"

static std::string machine_name = "small_test_model";
static std::string machine_dir = "../../data/" + machine_name;
static std::string resource_file_prefix = machine_dir + "/Resource";
static std::string recipe_dir = machine_dir + "/experiments/";

static const int num_resources = 15;

enum ModelSize {
	small,
	medium,
	big
};

static enum ModelSize model_size = ModelSize::small;

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

		if (i == 1) {
			if (model_size == ModelSize::small)
			{
				stream << "s0. o11. s1.\n";
				stream << "s0. o1. s1.\n";
				stream << "s1. out:1. s0.\n";
				stream << "s1. rem. s0.\n";
			}
			else if (model_size == ModelSize::medium || model_size == ModelSize::big)
			{
				stream << "s2. nop. s2.\n";
				stream << "s0. o11. s1.\n";
				stream << "s0. o1. s1.\n";
				stream << "s1. out:1. s0.\n";
				stream << "s1. rem. s0.\n";
				stream << "s1. o12. s2.\n";
				stream << "s1. o1a. s2.\n";
				stream << "s2. out:1. s0.\n";
				stream << "s2. rem. s0.\n";

				if (model_size == ModelSize::big)
				{
					stream << "s2. o13. s3.\n";
					stream << "s2. o1b. s3.\n";
					stream << "s3. out:1. s0.\n";
					stream << "s3. rem. s0.\n";
				}
			}
		} else {
			stream << "s0. o" + std::to_string(i) + "1. s1.\n";
			stream << "s0. in:" + std::to_string(i) + ". s1.\n";
			stream << "s1. out:" + std::to_string(i) + ". s0.\n";
			stream << "s1. rem. s0.\n";
			stream << "s1. o" + std::to_string(i) + "2. s2.\n";
			stream << "s1. o" + std::to_string(i) + ". s2.\n";
			stream << "s2. out:" + std::to_string(i) + ". s0.\n";
			stream << "s2. rem. s0.\n";

			if (model_size == ModelSize::medium || model_size == ModelSize::big)
			{
				stream << "s2. o" + std::to_string(i) + "3. s3.\n";
				stream << "s2. o" + std::to_string(i) + "a. s3.\n";
				stream << "s3. out:" + std::to_string(i) + ". s0.\n";
				stream << "s3. rem. s0.\n";

				if (model_size == ModelSize::big)
				{
					stream << "s3. o" + std::to_string(i) + "4. s4.\n";
					stream << "s3. o" + std::to_string(i) + "b. s4.\n";
					stream << "s4. out:" + std::to_string(i) + ". s0.\n";
					stream << "s4. rem. s0.\n";
				}
			}
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
			stream << "            \"output\": [\"p" + std::to_string(i) + "\'\"]\n";
			stream << "          }\n";
			stream << "        ],\n";
			stream << "        \"parallel\": []\n";
			stream << "      },\n";

			if (model_size == ModelSize::small)
			{
				stream << "      \"endState\": \"A" + std::to_string(i + 1) + "\"\n";
				stream << "    }";

				if (i < r) {
					stream << ",";
				}

				stream << "\n";
			}
			else if (model_size == ModelSize::medium || model_size == ModelSize::big)
			{
				stream << "      \"endState\": \"A" + std::to_string(i) + "\'\"\n";
				stream << "    },\n";

				stream << "    {\n";
				stream << "      \"startState\": \"A" + std::to_string(i) + "\'\",\n";
				stream << "      \"label\": {\n";
				stream << "        \"guard\": {},\n";
				stream << "        \"sequential\": [\n";
				stream << "          {\n";
				stream << "            \"name\": \"o" + std::to_string(i) + "a\",\n";
				stream << "            \"input\": [\"p" + std::to_string(i) + "\'\"";
				stream << "],\n";
				stream << "            \"parameters\": [],\n";

				if (model_size == ModelSize::medium)
				{
					stream << "            \"output\": [\"p" + std::to_string(i) + "\"]\n";
				}
				else if (model_size == ModelSize::big)
				{
					stream << "            \"output\": [\"p" + std::to_string(i) + "\'\'\"]\n";
				}
				
				stream << "          }\n";
				stream << "        ],\n";
				stream << "        \"parallel\": []\n";
				stream << "      },\n";

				if (model_size == ModelSize::medium)
				{
					stream << "      \"endState\": \"A" + std::to_string(i + 1) + "\"\n";
					stream << "    }";

					if (i < r) {
						stream << ",";
					}

					stream << "\n";
				}
				else if (model_size == ModelSize::big)
				{
					stream << "      \"endState\": \"A" + std::to_string(i) + "\'\'\"\n";
					stream << "    },\n";

					stream << "    {\n";
					stream << "      \"startState\": \"A" + std::to_string(i) + "\'\'\",\n";
					stream << "      \"label\": {\n";
					stream << "        \"guard\": {},\n";
					stream << "        \"sequential\": [\n";
					stream << "          {\n";
					stream << "            \"name\": \"o" + std::to_string(i) + "b\",\n";
					stream << "            \"input\": [\"p" + std::to_string(i) + "\'\'\"";
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
			}
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

static void CompleteTopology1(benchmark::State& state) {
	model_size = ModelSize::small;
	machine_name = "small_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	
	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopology(state, i, num_resources);
	}
}

//BENCHMARK(CompleteTopology1)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void ControllerUsingCompleteTopology1(benchmark::State& state) {
	model_size = ModelSize::small;
	machine_name = "small_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		ControllerUsingCompleteTopology(state, i, num_resources);
	}
}

//BENCHMARK(ControllerUsingCompleteTopology1)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void CompleteTopologyWithController1(benchmark::State& state) {
	model_size = ModelSize::small;
	machine_name = "small_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopologyWithController(state, i, num_resources);
	}
}

//BENCHMARK(CompleteTopologyWithController1)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void IncrementalTopologyWithController1(benchmark::State& state) {
	model_size = ModelSize::small;
	machine_name = "small_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		IncrementalTopologyWithController(state, i, num_resources);
	}
}

BENCHMARK(IncrementalTopologyWithController1)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void CompleteTopology2(benchmark::State& state) {
	model_size = ModelSize::medium;
	machine_name = "medium_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";

	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopology(state, i, num_resources);
	}
}

//BENCHMARK(CompleteTopology2)->DenseRange(2, num_resources - 3, 1)->Unit(benchmark::kMillisecond);

static void ControllerUsingCompleteTopology2(benchmark::State& state) {
	model_size = ModelSize::medium;
	machine_name = "medium_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		ControllerUsingCompleteTopology(state, i, num_resources);
	}
}

//BENCHMARK(ControllerUsingCompleteTopology2)->DenseRange(2, num_resources - 3, 1)->Unit(benchmark::kMillisecond);

static void CompleteTopologyWithController2(benchmark::State& state) {
	model_size = ModelSize::medium;
	machine_name = "medium_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopologyWithController(state, i, num_resources);
	}
}

//BENCHMARK(CompleteTopologyWithController2)->DenseRange(2, num_resources - 3, 1)->Unit(benchmark::kMillisecond);

static void IncrementalTopologyWithController2(benchmark::State& state) {
	model_size = ModelSize::medium;
	machine_name = "medium_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		IncrementalTopologyWithController(state, i, num_resources);
	}
}

BENCHMARK(IncrementalTopologyWithController2)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);

static void CompleteTopology3(benchmark::State& state) {
	model_size = ModelSize::big;
	machine_name = "big_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";

	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopology(state, i, num_resources);
	}
}

//BENCHMARK(CompleteTopology3)->DenseRange(2, num_resources - 3, 1)->Unit(benchmark::kMillisecond);

static void ControllerUsingCompleteTopology3(benchmark::State& state) {
	model_size = ModelSize::big;
	machine_name = "big_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		ControllerUsingCompleteTopology(state, i, num_resources);
	}
}

//BENCHMARK(ControllerUsingCompleteTopology3)->DenseRange(2, num_resources - 3, 1)->Unit(benchmark::kMillisecond);

static void CompleteTopologyWithController3(benchmark::State& state) {
	model_size = ModelSize::big;
	machine_name = "big_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		CompleteTopologyWithController(state, i, num_resources);
	}
}

//BENCHMARK(CompleteTopologyWithController3)->DenseRange(2, num_resources - 3, 1)->Unit(benchmark::kMillisecond);

static void IncrementalTopologyWithController3(benchmark::State& state) {
	model_size = ModelSize::big;
	machine_name = "big_test_model";
	machine_dir = "../../data/" + machine_name;
	resource_file_prefix = machine_dir + "/Resource";
	recipe_dir = machine_dir + "/experiments/";

	for (size_t i = 2; i <= state.range(0); ++i) {
		IncrementalTopologyWithController(state, i, num_resources);
	}
}

BENCHMARK(IncrementalTopologyWithController3)->DenseRange(2, num_resources, 1)->Unit(benchmark::kMillisecond);