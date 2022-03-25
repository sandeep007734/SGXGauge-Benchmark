#Publication

This work is published in ISPASS 2022.

We present SGXGauge, a comprehensive benchmark suite for Intel SGX. SGXGauge contains 10 real-world and synthetic benchmarks from different domains that thoroughly evaluate all the critical components of Intel SGX. The following table provides a description of the SGXGauge workloads, as well as the specific settings used in the paper:

| Workloads | Ported | LibOS |
| :------------ |:---:| ---:|
| Blockchain | ✓ | ✓ |
| OpenSSL | ✓ | ✓ |
| B-Tree | ✓ | ✓ |
| HashJoin | ✓ | ✓ |
| BFS | ✓ | ✓ |
| PageRank | ✓ | ✓ |
| Memcached | ✗ | ✓ |
| XSBench | ✗ | ✓ |
| Lighttpd | ✗ | ✓ |
| SVM | ✗ | ✓ |

Furthermore, each of the benchmark is configured to have three input settings: LOW_, HIGH_ and MEDIUM_. A detailed description of each of these setting is given below:

| Setting | Memory Footprint |
| :------- |:----------------:|
| LOW_ | < EPC |
| MEDIUM_ | = EPC |
| HIGH_ | > EPC |
Note: EPC is considered to 92MB.

#Organization
The following convention is used to denote each directory:
<benchmark-name>-native: The ported version of the corresponding benchmark is contained in this directory.
<benchmark-name>-vanilla: The non-SGX and LibOS versions of the corresponding benchmark are contained in this directory.

#Execution Instructions
Only non-SGX and LibOS versions are available for the following benchmarks: Lighttpd, SVM, XSBench, and Memcached. Please refer to the following guidelines to execute it:

1. To execute LibOS version of any of the benchmarks, you need to execute the following command from the correspoding workload's directory: sudo -H -E ./run_local.sh 1 <Setting>

2. To execute non-SGX version of any of the benchmarks, you need to execute the following command from the correspoding workload's directory: sudo -H -E ./run_local.sh 3 <Setting>

3. Refer run_local.sh script present in the benchmark's directory to adjust the input size.

Ported, non-SGX and LibOS versions are available for the following benchmarks: BlockChain, OpenSSL, B-Tree, HashJoin, BFS and PageRank. Please refer to the following guidelines to execute it:
1. Each of the benchmark contains a config.h file which defines the input settings for a benchmark.
2. To generate graphs for BFS and Pagerank workloads execute the following commands to generate the testcases: ./bin/graph_gen and  ./bin/page_gen in bfs-2-vanilla and pagerank-2-vanilla directories. Copy the input files to bfs-2-native and pagerank-2-native directories respectively.
3. To generate file for OpenSSL workload execute the following command: bash setup.sh  in openssl-vanilla directory. Copy the input files to openssl-native directory respectively.
4. To execute the LibOS version of any of the benchmarks, you need to execute the following command from the correspoding workload's directory: sudo -H -E ./run_local.sh 1 <Setting>.
5. To execute the non-SGX version of any of the benchmarks, you need to execute the following command from the correspoding workload's directory: sudo -H -E ./run_local.sh 3 <Setting>.
6. To execute the ported version of any of the benchmarks, you need to execute the following command from the correspoding workload's directory: sudo -H -E ./run_local.sh 4 <Setting>.
