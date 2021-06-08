Breadth-first search
====================

In Breadth-first search, a graph (comprised of vertices and edges) is read in
from a file.  From this, a single-source shortest-path algorithm is computed.
This algorithm currently assumes that each edge weight is one.  However, the
algorithm should be easily extendable to cover this case.  This particular
program has been ported from the Rodinia Benchmark Suite. 

Note: This application was ported from the Rodinia Suite
      (https://www.cs.virginia.edu/~skadron/wiki/rodinia/).

# Installation

During installation, the [install](./install) script uses Wu-Wei to compile the C++
implementation, and pregenerate inputs to speedup the execution of the Matlab
implementation. This is not strictly necessary to execute the Matlab version
but failure to do so can make its execution incredibly slow since the
generation of the data is really slow itself. Several hundreds of megabytes are 
saved on disk in the [input directory](./input). If you know you won't use the Matlab implementation, 
you can safely delete the generated files.

Should the installation script fails for any reason, you can manually generate
the input data by following these steps.

  1. Compile the C++ implementation, and note the compilation destination directory:
  ````wu build bfs cpp -v````
  The output should be something similar to ````result saved in /.../builds/33c2d...````.
   2. Run the compiled implementation with two extra arguments to save the generated input in the corresponding files for all input sizes defined in benchmark.json:
  ````/.../builds/33c2d.../runner $BENCHMARK_INPUT_SIZE ./input/$BENMARK_INPUT_SIZE'-vertices.csv' ./input/$BENMARK_INPUT_SIZE'-edges.csv'````  

# Notes

The C++ and Matlab implementations have been tested for the small, medium, and large sizes and run in a reasonable time. The JavaScript implementation has been tested on the small, and medium sizes and the initialization of the benchmark taskes a significant amount of time. It took too long to run in JavaScript so it was never tested to run to completion as it triggers the "unresponsive script" on browsers. Moreover it may generate too much memory to handle in memory during generation and loading the data from a file was too complicated to handle.

The benchmark has been tested on the following configurations on a MacBook Air 2011 running OSX 10.11.6:

| implementation | compiler      | environment | input-size | mean      | std     | min       | max       | repetitions |
| -------------- | ------------- | ----------- | ---------- | --------- | ------- | --------- | --------- | ----------- |
| matlab         | matlab-concat | matlab-vm   | small      | 0.0101s   | +-0.00% | 0.0101s   | 0.0101s   | 1           |
| matlab         | matlab-concat | octave-4.0  | small      | 0.7153s   | +-0.00% | 0.7153s   | 0.7153s   | 1           |
| js             | browserify    | node        | small      | 0.0020s   | +-0.00% | 0.0020s   | 0.0020s   | 1           |
| js             | browserify    | firefox     | small      | 0.0095s   | +-0.00% | 0.0095s   | 0.0095s   | 1           |
| js             | browserify    | safari      | small      | 0.0163s   | +-0.00% | 0.0163s   | 0.0163s   | 1           |
| matlab         | matlab-concat | octave-4.0  | medium     | 966.9412s | +-0.00% | 966.9412s | 966.9412s | 1           |
| matlab         | matlab-concat | matlab-vm   | medium     | 2.9096s   | +-0.00% | 2.9096s   | 2.9096s   | 1           |
| js             | browserify    | node        | medium     | 1.2730s   | +-0.00% | 1.2730s   | 1.2730s   | 1           |
| js             | browserify    | chrome      | medium     | 3.6454s   | +-0.00% | 3.6454s   | 3.6454s   | 1           |
| cpp            | g++           | native      | small      | 0.0002s   | +-0.00% | 0.0002s   | 0.0002s   | 1           |
| js             | browserify    | firefox     | medium     | 1.1293s   | +-0.00% | 1.1293s   | 1.1293s   | 1           |
| cpp            | g++           | native      | medium     | 0.5063s   | +-0.00% | 0.5063s   | 0.5063s   | 1           |
| cpp            | g++           | native      | large      | 3.3158s   | +-0.00% | 3.3158s   | 3.3158s   | 1           |
| matlab         | matlab-concat | matlab-vm   | large      | 13.3932s  | +-0.00% | 13.3932s  | 13.3932s  | 1           |
| js             | browserify    | safari      | medium     | 3.0534s   | +-0.00% | 3.0534s   | 3.0534s   | 1           |
| js             | browserify    | chrome      | small      | 0.0124s   | +-0.00% | 0.0124s   | 0.0124s   | 1           |

