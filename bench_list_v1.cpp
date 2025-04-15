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
    std::array<std::byte, 4 * kPageSize> padding{};
};

Node* walk(Node * p, uint64_t ops) {
    // how will change the latency with atomic?
    // std::atomic<uint64_t> value{};

    while (ops > 0) {
        p = p->next;
        // value.fetch_add(1);
        // benchmark::DoNotOptimize(value);
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

    std::map<uint32_t, int> unique_patterns;
    for (std::size_t i=0; i<buffer.size() - 1; i++) {
        buffer[i].next = &buffer[i+1];

        uint64_t ptr_value = reinterpret_cast<uint64_t>(&buffer[i]);
        const uint64_t mask = (1 << 6) - 1;
        uint32_t bit_pattern = (ptr_value >> 4) & mask;
        unique_patterns[bit_pattern]++;
    }
    for (const auto& [k, v] : unique_patterns) {
        std::cout << "k=" << k << ", count=" << v << std::endl;
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
    // ->Range(1, 32)
    // ->DenseRange(48, 160, 16)
    // // Kb here is confusing :)
    // ->Range(256, 1_KB)
    // ->DenseRange(1_KB, 4_KB, 512)
    // ->Range(4_KB, 16_KB)
    // ->DenseRange(20_KB, 48_KB, 4_KB)
    ->Arg(48_KB)
    ;

BENCHMARK_MAIN();