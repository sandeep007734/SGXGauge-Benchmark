
#ifdef high
    #define MIN_NODES      20
    #define MAX_NODES      ULONG_MAX
    #define MIN_EDGES      50
    #define MAX_INIT_EDGES 100
    #define MIN_WEIGHT     1
    #define MAX_WEIGHT     10
    #define NUM_NODES      200000
    #define GRAPH_FILE_PATH "/tmp/vertex_high.dat"

#elif low
    #define MIN_NODES      20
    #define MAX_NODES      ULONG_MAX
    #define MIN_EDGES      20
    #define MAX_INIT_EDGES 30
    #define MIN_WEIGHT     1
    #define MAX_WEIGHT     10
    #define NUM_NODES      200000
    #define GRAPH_FILE_PATH "/tmp/vertex_low.dat"

#else
    #define MIN_NODES      20
    #define MAX_NODES      ULONG_MAX
    #define MIN_EDGES      30
    #define MAX_INIT_EDGES 60
    #define MIN_WEIGHT     1
    #define MAX_WEIGHT     10
    #define NUM_NODES      200000
    #define GRAPH_FILE_PATH "/tmp/vertex_medium.dat"
#endif
