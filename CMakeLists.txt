cmake_minimum_required(VERSION 3.15)
project(memory_latency_benchmark)

add_compile_options("-g")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(benchmark)

add_executable(bench_loop bench_loop.cpp)
add_executable(bench_list_v1 bench_list_v1.cpp)
add_executable(bench_list_v2 bench_list_v2.cpp)

target_link_libraries(bench_loop PRIVATE benchmark::benchmark pthread)
target_link_libraries(bench_list_v1 PRIVATE benchmark::benchmark pthread)
target_link_libraries(bench_list_v2 PRIVATE benchmark::benchmark pthread)