#include <benchmark/benchmark.h>
#include "pcs/controller/topology.h"

#include <array>

#include "pcs/lts/parsers.h"

static void BM_TopoologyWithFiveResources(benchmark::State& state) {
    std::array<pcs::LabelledTransitionSystem<std::string, std::string>, 5> ltss;
    for (size_t i = 0; i < 5; ++i) {
        pcs::ReadFromFile(ltss[i], "../../data/pad/Resource" + std::to_string(i + 1) + ".txt");
    }
    
    for (auto _ : state) {
        pcs::Combine(ltss);
    }
}

BENCHMARK(BM_TopoologyWithFiveResources);
