#include <memory>
#include <benchmark/benchmark.h>

class CustomMemoryManager : public benchmark::MemoryManager {
public:
    int64_t num_allocs;
    int64_t max_bytes_used;
public:
    void Start() BENCHMARK_OVERRIDE {
        num_allocs = 0;
        max_bytes_used = 0;
    }

    void Stop(Result* result) BENCHMARK_OVERRIDE {
        result->num_allocs = num_allocs;
        result->max_bytes_used = max_bytes_used;
    }
};