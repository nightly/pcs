#pragma once 

#include <memory>
#include <benchmark/benchmark.h>

class CustomMemoryManager : public benchmark::MemoryManager {
public:
    int64_t num_allocs;
    int64_t max_bytes_used;
public:
    void Start() override {
        num_allocs = 0;
        max_bytes_used = 0;
    }

    void Stop(Result* result) BENCHMARK_OVERRIDE {
        result->num_allocs = num_allocs;
        result->max_bytes_used = max_bytes_used;
    }
};

inline std::unique_ptr<CustomMemoryManager> mm(new CustomMemoryManager());

#ifdef MEMORY_PROFILER
inline void* custom_malloc(size_t size) {
    void* p = malloc(size);
    if (mm.get() != nullptr) {
        mm.get()->num_allocs += 1;
        mm.get()->max_bytes_used += size;
    }
    return p;
}
#define malloc(size) custom_malloc(size)

inline void* operator new(size_t size)
{
    void* p = malloc(size);
    return p;
}

inline void operator delete(void* p) noexcept
{
    free(p);
}

inline void* operator new[](size_t size)
{
    void* p = malloc(size);
    return p;
}

inline void operator delete[](void* p) noexcept
{
    free(p);
}
#endif