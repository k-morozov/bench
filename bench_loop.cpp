#include <cstdint>
#include <vector>
#include <random>

#include "benchmark/benchmark.h"


constexpr size_t WAYS_OF_ASSOCIATIVITY = 12;
constexpr size_t NUMBER_OF_SET = 64;
constexpr size_t COHERENCY_LINE_SIZE = 64;

const size_t STRIDE = 4096;
const size_t SUB_STRIDE = 64;

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

    while (state.KeepRunningBatch(page_count)) {
        for (size_t page = 0; page < page_count; ++page) {
            size_t offset = page * STRIDE;
            sum += buf[offset];

            benchmark::DoNotOptimize(sum);
        }
    }
}

static void LoopOffset(benchmark::State& state) {
    size_t page_count = state.range(0);
    const size_t buf_size = page_count * STRIDE;
    std::vector<uint8_t> buf (buf_size, 0);
    fill_random(buf, buf_size);
    benchmark::DoNotOptimize(buf);

    size_t sum = 0;

    while (state.KeepRunningBatch(page_count)) {
        for (size_t page = 0; page < page_count; ++page) {
            size_t offset = page * STRIDE + (page * SUB_STRIDE) % STRIDE;
            sum += buf[offset];
            benchmark::DoNotOptimize(sum);
        }
    }
}

BENCHMARK(SimpleLoop)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(NUMBER_OF_SET*WAYS_OF_ASSOCIATIVITY)
    ;

BENCHMARK(LoopOffset)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(NUMBER_OF_SET*WAYS_OF_ASSOCIATIVITY)
    ;

BENCHMARK(SimpleLoop)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(NUMBER_OF_SET*WAYS_OF_ASSOCIATIVITY*COHERENCY_LINE_SIZE)
    ;

BENCHMARK(LoopOffset)
    ->ArgName("Pages")
    ->Repetitions(1)
    ->Arg(NUMBER_OF_SET*WAYS_OF_ASSOCIATIVITY*COHERENCY_LINE_SIZE)
    ;

BENCHMARK_MAIN();