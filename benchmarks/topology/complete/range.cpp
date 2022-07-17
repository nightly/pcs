#include <benchmark/benchmark.h>
#include "pcs/topology/complete.h"

#include <array>

#include "lts/parsers/parsers.h"

static void BM_TopologyRange(benchmark::State& state) {
    std::vector<nightly::LTS<std::string, std::string>> ltss;
    ltss.resize(state.range(0));
    for (size_t i = 0; i < state.range(0); ++i) {
        nightly::ReadFromFile(ltss[i], "../../data/pad/Resource1.txt");
    }

    for (auto _ : state) {
        pcs::CompleteTopology topology(ltss);
        benchmark::DoNotOptimize(topology);
        benchmark::ClobberMemory();
    }
}

// BENCHMARK(BM_TopologyRange)->Arg(6)->Iterations(1000)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_TopologyRange)->DenseRange(2, 10, 1)->Unit(benchmark::kMillisecond);
