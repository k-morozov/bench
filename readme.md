```shell
cmake .. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -DCMAKE_BUILD_TYPE=Release && cmake --build . && sudo perf stat -B -e L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,l2_request.all,l2_request.miss,LLC-loads,LLC-load-misses,cache-references,cache-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,dtlb_load_misses.walk_completed,dtlb_store_misses.walk_completed,itlb_misses.miss_caused_walk,dtlb_load_misses.stlb_hit,dtlb_load_misses.walk_completed,dtlb_load_misses.walk_completed_4k,dtlb_store_misses.stlb_hit,page-faults -r 1 ./bench_list
```


## Results

```shell
Run on (32 X 5200 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 2048 KiB (x16)
  L3 Unified 36864 KiB (x1)
Load Average: 0.57, 0.57, 0.52

------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
MemoryLatencyList/size KB:1          0.930 ns        0.930 ns    749731840 Nodes=16 Read Rate=64.0898Gi/s
MemoryLatencyList/size KB:2          0.930 ns        0.930 ns    752877568 Nodes=32 Read Rate=64.0962Gi/s
MemoryLatencyList/size KB:4          0.930 ns        0.930 ns    751828992 Nodes=64 Read Rate=64.0984Gi/s
MemoryLatencyList/size KB:8           2.23 ns         2.23 ns    314572800 Nodes=128 Read Rate=26.7171Gi/s
MemoryLatencyList/size KB:16          2.23 ns         2.23 ns    313524224 Nodes=256 Read Rate=26.6955Gi/s
MemoryLatencyList/size KB:32          2.23 ns         2.23 ns    313524224 Nodes=512 Read Rate=26.6855Gi/s
MemoryLatencyList/size KB:48          3.59 ns         3.59 ns    195035136 Nodes=768 Read Rate=16.6047Gi/s
MemoryLatencyList/size KB:64          4.28 ns         4.28 ns    164626432 Nodes=1Ki Read Rate=13.9149Gi/s
MemoryLatencyList/size KB:128         5.32 ns         5.32 ns    128974848 Nodes=2Ki Read Rate=11.2082Gi/s
MemoryLatencyList/size KB:256         6.65 ns         6.65 ns    104857600 Nodes=4Ki Read Rate=8.96333Gi/s
MemoryLatencyList/size KB:512         6.72 ns         6.72 ns    104857600 Nodes=8Ki Read Rate=8.87073Gi/s
MemoryLatencyList/size KB:1024        6.89 ns         6.89 ns    101711872 Nodes=16Ki Read Rate=8.64681Gi/s
MemoryLatencyList/size KB:2048        12.9 ns         12.9 ns     51380224 Nodes=32Ki Read Rate=4.62378Gi/s
MemoryLatencyList/size KB:4096        16.4 ns         16.4 ns     42991616 Nodes=64Ki Read Rate=3.64468Gi/s
MemoryLatencyList/size KB:8192        16.5 ns         16.5 ns     41943040 Nodes=128Ki Read Rate=3.62015Gi/s
MemoryLatencyList/size KB:16384       18.7 ns         18.7 ns     38797312 Nodes=256Ki Read Rate=3.19403Gi/s
MemoryLatencyList/size KB:32768       39.2 ns         39.2 ns     16777216 Nodes=512Ki Read Rate=1.5203Gi/s
MemoryLatencyList/size KB:36864       84.7 ns         84.7 ns     11534336 Nodes=576Ki Read Rate=720.346Mi/s
MemoryLatencyList/size KB:36864       78.6 ns         78.6 ns      8388608 Nodes=576Ki Read Rate=776.692Mi/s
MemoryLatencyList/size KB:65536       75.4 ns         75.4 ns      9437184 Nodes=1Mi Read Rate=809.183Mi/s
MemoryLatencyList/size KB:73728       80.6 ns         80.6 ns      9437184 Nodes=1.125Mi Read Rate=757.712Mi/s
```