```shell
Run on (32 X 5200 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 2048 KiB (x16)
  L3 Unified 36864 KiB (x1)
Load Average: 0.26, 0.36, 0.41
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
SimpleLoop/Pages:768/repeats:1         2.04 ns         2.04 ns    345076224
LoopOffset/Pages:768/repeats:1         1.42 ns         1.42 ns    487831296
SimpleLoop/Pages:49152/repeats:1       11.6 ns         11.6 ns     57753600
LoopOffset/Pages:49152/repeats:1       2.30 ns         2.30 ns    301694976
```