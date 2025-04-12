#include <array>
#include <cstdint>
#include <vector>

#include "benchmark/benchmark.h"

constexpr std::size_t kCachelineSize = 1 << 6;
constexpr std::size_t kPageSize = 1 << 12;

auto constexpr operator""_KB(unsigned long long int n) { return n * 1024; }
auto constexpr operator""_MB(unsigned long long int n) { return n * 1024 * 1024; }

struct Node {
    Node* next{};
    std::array<std::byte, kPageSize> padding;
};

Node* walk(Node * p, uint64_t ops) {
    while (ops > 0) {
        p = p->next;
        --ops;
    }
    return p;
}


static void MemoryLatencyList(benchmark::State& state) {
    const std::size_t mem_block_size = operator""_KB(state.range(0));
    assert(mem_block_size > 0);

    const std::size_t num_nodes = mem_block_size / kCachelineSize;

    assert(num_nodes > 0);

    std::vector<Node> buffer(num_nodes);

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
    ->Range(256, 1_KB)
    ->DenseRange(1_KB, 4_KB, 512)

    ->Range(4_KB, 16_KB)
    ->DenseRange(20_KB, 48_KB, 4_KB)
    ;

BENCHMARK_MAIN();