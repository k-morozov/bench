#include <cstdint>
#include <iostream>
#include <vector>
#include <random>

#include "benchmark/benchmark.h"


const size_t STRIDE = 4096;
const size_t SUB_STRIDE = 64;
constexpr size_t CACHE_LINE = 64;
const size_t page_count = 4096;

size_t loop_simple(const std::vector<uint8_t>& buf, size_t count) {
    size_t sum = 0;

    for (size_t page = 0; page < count; ++page) {
        size_t offset = page * STRIDE;
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
    size_t page_count = state.range(0);
    const size_t buf_size = page_count * STRIDE;

    std::vector<uint8_t> buf (buf_size, 0);

    fill_random(buf, buf_size);
    benchmark::DoNotOptimize(buf);

    size_t sum = 0;

    // std::map<uint32_t, int> unique_patterns;

    while (state.KeepRunningBatch(page_count)) {
        for (size_t page = 0; page < page_count; ++page) {
            size_t offset = page * STRIDE;
            sum += buf[offset];

            // uint64_t ptr_value = reinterpret_cast<uint64_t>(&buf[offset]);
            // const uint64_t mask = (1 << 6) - 1;
            // uint32_t bit_pattern = (ptr_value >> 4) & mask;
            // unique_patterns[bit_pattern]++;

            benchmark::DoNotOptimize(sum);
        }
    }

    // for (const auto& [k, v] : unique_patterns) {
    //     std::cout << "k=" << k << ", count=" << v << std::endl;
    // }

    // std::cout << "===" << std::endl;
}

static void LoopOffset(benchmark::State& state) {
    size_t page_count = state.range(0);
    const size_t buf_size = page_count * STRIDE;

    std::vector<uint8_t> buf (buf_size, 0);

    fill_random(buf, buf_size);
    benchmark::DoNotOptimize(buf);

    size_t sum = 0;

    // std::map<uint32_t, int> unique_patterns;

    while (state.KeepRunningBatch(page_count)) {
        for (size_t page = 0; page < page_count; ++page) {
            size_t offset = page * STRIDE + (page * SUB_STRIDE) % STRIDE;
            sum += buf[offset];

            // uint64_t ptr_value = reinterpret_cast<uint64_t>(&buf[offset]);
            // const uint64_t mask = (1 << 6) - 1;
            // uint32_t bit_pattern = (ptr_value >> 4) & mask;
            // unique_patterns[bit_pattern]++;

            benchmark::DoNotOptimize(sum);
        }
    }

    // for (const auto& [k, v] : unique_patterns) {
    //     std::cout << "k=" << k << ", count=" << v << std::endl;
    // }

    // std::cout << "===" << std::endl;
}

BENCHMARK(SimpleLoop)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(64*12*64)
    // ->Arg(2048)
    // ->Arg(4096)
    // ->Arg(8000)
    ;

BENCHMARK(LoopOffset)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(64*12*64)
    ;

BENCHMARK_MAIN();