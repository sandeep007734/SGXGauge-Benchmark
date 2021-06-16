
#ifdef HIGH_
    #define MIN_NODES      20
    #define MAX_NODES      ULONG_MAX
    #define MIN_EDGES      3
    #define MAX_INIT_EDGES 10
    #define MIN_WEIGHT     1
    #define MAX_WEIGHT     10
    #define NUM_NODES      150000
    #define GRAPH_FILE_PATH "vertex_high.dat"

#elif LOW_
    #define MIN_NODES      20
    #define MAX_NODES      ULONG_MAX
    #define MIN_EDGES      3
    #define MAX_INIT_EDGES 10
    #define MIN_WEIGHT     1
    #define MAX_WEIGHT     10
    #define NUM_NODES      70000
    #define GRAPH_FILE_PATH "vertex_low.dat"

#else
    #define MIN_NODES      20
    #define MAX_NODES      ULONG_MAX
    #define MIN_EDGES      3
    #define MAX_INIT_EDGES 10
    #define MIN_WEIGHT     1
    #define MAX_WEIGHT     10
    #define NUM_NODES      100000
    #define GRAPH_FILE_PATH "vertex_medium.dat"
#endif