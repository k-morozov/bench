#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "benchmark/benchmark.h"

auto constexpr operator""_B(unsigned long long int n) { return n; }
auto constexpr operator""_KB(unsigned long long int n) { return n * 1024; }
auto constexpr operator""_MB(unsigned long long int n) { return n * 1024 * 1024; }
auto constexpr operator""_K(unsigned long long int n) { return n * 1000; }


constexpr std::size_t kCachelineSize = 64_B;
constexpr std::size_t kPageSize = 4_KB;

struct Node {
    Node* next{};
    std::array<std::byte, kPageSize> padding{};
};

Node* walk(Node * p, uint64_t ops) {
    while (ops > 0) {
        p = p->next;
        ops--;

        benchmark::DoNotOptimize(p);
        benchmark::DoNotOptimize(ops);
    }

    return p;
}


static void MemoryLatencyList(benchmark::State& state) {
    const std::size_t mem_block_size = operator""_KB(state.range(0));
    assert(mem_block_size > 0);

    const std::size_t num_nodes = mem_block_size / kCachelineSize;

    assert(num_nodes > 0);

    std::vector<Node> buffer(num_nodes);
    benchmark::DoNotOptimize(buffer);

    for (std::size_t i=0; i<buffer.size() - 1; i++) {
        buffer[i].next = &buffer[i+1];
    }
    
    buffer.back().next = &buffer.front();

    constexpr size_t num_ops = 1 << 20;
    while (state.KeepRunningBatch(num_ops)) { 
        auto last_node = walk(&buffer.front(), num_ops);
        benchmark::DoNotOptimize(last_node);
    }

    state.counters["Nodes"] =
    benchmark::Counter(num_nodes, benchmark::Counter::kDefaults,
                       benchmark::Counter::OneK::kIs1024);
}

BENCHMARK(MemoryLatencyList)
    ->ArgName("size KB")
    // ->Repetitions(4)
    ->RangeMultiplier(2)
    ->Range(1, 32)
    ->DenseRange(48, 160, 16)
    ->Range(256, 1_K)
    ->DenseRange(1_K, 4_K, 512)
    ->Range(4_K, 16_K)
    ->DenseRange(20_K, 48_K, 4_K)
    ->Arg(48_K)
    ;

BENCHMARK_MAIN();