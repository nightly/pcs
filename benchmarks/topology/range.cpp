#include <benchmark/benchmark.h>
#include "pcs/topology/topology.h"

#include <array>

#include "pcs/lts/parsers/string_string.h"

static void BM_TopologyRange(benchmark::State& state) {
    std::vector<pcs::LTS<std::string, std::string>> ltss;
    ltss.resize(state.range(0));
    for (size_t i = 0; i < state.range(0); ++i) {
        pcs::ReadFromFile(ltss[i], "../../data/pad/Resource1.txt");
    }

    for (auto _ : state) {
        pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> topology = pcs::Combine(ltss);
        benchmark::DoNotOptimize(topology);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_TopologyRange)->Arg(3)->Iterations(10);
// Can use ->Ranges() but requires a multiplier (?)