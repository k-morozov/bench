#include <cstdint>
#include <iostream>
#include <vector>
#include <random>

#include "benchmark/benchmark.h"


const size_t STRIDE = 4096;
const size_t SUB_STRIDE = 64;
constexpr size_t CACHE_LINE = 64;
const size_t page_count = 4096;
const size_t buf_size = page_count * STRIDE;

size_t loop_simple(const std::vector<uint8_t>& buf, size_t count) {
    size_t sum = 0;

    for (size_t page = 0; page < count; ++page) {
        size_t offset = page * STRIDE;
        sum += buf[offset];
    }
    return sum;
}

size_t loop_with_offset(const std::vector<uint8_t>& buf, size_t count) {
    size_t sum = 0;
    for (size_t page = 0; page < count; ++page) {
        size_t offset = page * STRIDE + (page * SUB_STRIDE) % STRIDE;
        sum += buf[offset];
    }
    return sum;
}

void fill_random(std::vector<uint8_t>& buf, size_t count) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 249);
    for (size_t i = 0; i < count; ++i) {
        buf[i] = static_cast<uint8_t>(dist(rng));
    }
}

static void SimpleLoop(benchmark::State& state) {
    std::vector<uint8_t> buf (buf_size, 0);

    fill_random(buf, buf_size);
    benchmark::DoNotOptimize(buf);

    size_t sum = 0;

    size_t num_ops = state.range(0);
    while (state.KeepRunningBatch(num_ops)) {
        for (size_t page = 0; page < num_ops; ++page) {
            size_t offset = page * STRIDE;
            sum += buf[offset];

            benchmark::DoNotOptimize(sum);
        }
    }
}

BENCHMARK(SimpleLoop)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(4096)
    ;

BENCHMARK_MAIN();