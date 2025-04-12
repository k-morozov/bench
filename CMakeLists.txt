cmake_minimum_required(VERSION 3.15)
project(memory_latency_benchmark)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
# set(BENCHMARK_ENABLE_TESTING OFF)

# add_compile_options(BENCHMARK_ENABLE_GTEST_TESTS OFF)

add_subdirectory(benchmark)

add_executable(bench_list bench_list.cpp)

target_link_libraries(bench_list PRIVATE benchmark::benchmark pthread)