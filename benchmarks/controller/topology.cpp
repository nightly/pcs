#include <benchmark/benchmark.h>
#include "pcs/topology/topology.h"

#include <array>

#include "pcs/lts/parsers/string_string.h"
#include "pcs/common/directory.h"

static void BM_TopoologyWithFiveResources(benchmark::State& state) {
    std::array<pcs::LabelledTransitionSystem<std::string, std::string>, 5> ltss;
    for (size_t i = 0; i < 5; ++i) {
        pcs::ReadFromFile(ltss[i], "../../data/pad/Resource" + std::to_string(i + 1) + ".txt");
    }
    
    for (auto _ : state) {
        pcs::LabelledTransitionSystem<std::string, std::string> topology = pcs::Combine(ltss);
        benchmark::DoNotOptimize(topology);
    }
}

BENCHMARK(BM_TopoologyWithFiveResources);

// 414915 ns - 1659 iterations