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
        //std::cout << "State Count = " << topology.lts().NumOfStates() << std::endl;
        //std::cout << "Transition Count = " << topology.lts().NumOfTransitions() << std::endl;
        benchmark::DoNotOptimize(topology);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_TopologyRange)->Arg(8)->Iterations(25);
// Can use ->Ranges() instad of Arg() but requires a multiplier (?)