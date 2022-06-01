#include <benchmark/benchmark.h>
#include "pcs/topology/complete.h"

#include <array>

#include "pcs/lts/parsers/string_string.h"

static void BM_TopologyRange(benchmark::State& state) {
    std::vector<pcs::LTS<std::string, std::string>> ltss;
    ltss.resize(state.range(0));
    for (size_t i = 0; i < state.range(0); ++i) {
        pcs::ReadFromFile(ltss[i], "../../data/pad/Resource1.txt");
    }

    for (auto _ : state) {
        pcs::CompleteTopology topology(ltss);
        benchmark::DoNotOptimize(topology);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_TopologyRange)->Arg(6)->Iterations(1000)->Unit(benchmark::kMillisecond);
// Can use ->Ranges() instad of Arg() but requires a multiplier (?)