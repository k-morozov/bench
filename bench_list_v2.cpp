#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <random>

#include "benchmark/benchmark.h"

constexpr std::size_t kCachelineSize = 1 << 6;
constexpr std::size_t kPageSize = 1 << 12;

auto constexpr operator""_KB(unsigned long long int n) { return n * 1024; }
auto constexpr operator""_MB(unsigned long long int n) { return n * 1024 * 1024; }

struct Node {
    Node* next{};
    std::array<std::byte, kPageSize> padding{};
};

Node* walk(Node * p, uint64_t ops) {    
    std::atomic<uint64_t> value{};
    while (ops > value.load()) {
        p = p->next;
        value.fetch_add(1);
        benchmark::DoNotOptimize(value);
        benchmark::DoNotOptimize(p);
    }

    return p;
}


uint64_t reverse_bits(uint64_t n) {
    uint64_t reversed = 0;
    for (int i = 0; i < 64; ++i) {
        reversed |= ((n >> i) & 1) << (63 - i);
    }
    return reversed;
}

bool isBigEndian() {
    uint16_t number = 0x1;
    char* byte = reinterpret_cast<char*>(&number);
    return byte[0] == 0;
}

static void MemoryLatencyList(benchmark::State& state) {
    const std::size_t mem_block_size = operator""_KB(state.range(0));
    assert(mem_block_size > 0);

    const std::size_t num_nodes = mem_block_size / kCachelineSize;

    assert(num_nodes > 0);

    // std::vector<Node> buffer(num_nodes);
    // benchmark::DoNotOptimize(buffer);

    // for (std::size_t i=0; i<buffer.size() - 1; i++) {
    //     buffer[i].next = &buffer[i+1];
    // }
    // buffer.back().next = &buffer.front();


    Node* head = new Node{};
    benchmark::DoNotOptimize(head);

    Node * tail = head;
    benchmark::DoNotOptimize(tail);
    std::map<uint32_t, int> unique_patterns;
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(50090, 60000);
    std::uniform_int_distribution<int> dist2(0,1000);

    std::vector<void*> offsets(num_nodes);

    for (std::size_t i=0; i<num_nodes; i++) {
        
        int* offset = (int*)malloc(dist(rng));

        offset[dist2(rng)] = dist(rng);
        offsets.push_back(offset);
        benchmark::DoNotOptimize(offset);

        uint64_t ptr_value = reinterpret_cast<uint64_t>(tail);
        const uint64_t mask = (1 << 6) - 1;
        uint32_t bit_pattern = (ptr_value >> 4) & mask;

        // std::cout << "ptr_value=" << ptr_value << ", tail=" << tail << ", bit_pattern=[" << bit_pattern << "]\n" << std::endl;

        unique_patterns[bit_pattern]++;

        Node* node = new Node{};
        benchmark::DoNotOptimize(node);
        tail->next = node;
        tail = node;
    }

    tail->next = head;
    for (const auto& [k, v] : unique_patterns) {
        std::cout << "k=" << k << ", count=" << v << std::endl;
    }
    std::cout << "isBigEndian=" << isBigEndian() << ", unique_patterns=" << unique_patterns.size() << std::endl;

    constexpr size_t num_ops = 1 << 20;
    while (state.KeepRunningBatch(num_ops)) { 
        auto last_node = walk(head, num_ops);
        benchmark::DoNotOptimize(last_node);
    }

    state.counters["Nodes"] =
    benchmark::Counter(num_nodes, benchmark::Counter::kDefaults,
                       benchmark::Counter::OneK::kIs1024);

    tail->next = nullptr;
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }

    for (auto p : offsets) {
        free(p);
    }
}

BENCHMARK(MemoryLatencyList)
    ->ArgName("size KB")
    // ->Repetitions(4)
    ->RangeMultiplier(2)
    // ->Range(1, 32)
    // ->DenseRange(48, 160, 16)
    // ->Range(256, 1_KB)
    // ->DenseRange(1_KB, 4_KB, 512)
    // ->Range(4_KB, 16_KB)
    // ->DenseRange(20_KB, 48_KB, 4_KB)
    ->Arg(96)
    ;

BENCHMARK_MAIN();