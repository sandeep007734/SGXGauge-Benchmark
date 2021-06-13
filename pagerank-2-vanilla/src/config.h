#ifdef high
    #define D_FACTOR       0.85
    #define MAX_ITER       1000
    #define THRESHOLD      0.00001
    #define NUM_PAGES      15000
    #define DIVISOR        2
    #define PAGE_FILE_PATH "/tmp/pages_high.dat"

#elif low
    #define D_FACTOR       0.85
    #define MAX_ITER       1000
    #define THRESHOLD      0.00001
    #define NUM_PAGES      7500
    #define DIVISOR        2
    #define PAGE_FILE_PATH "/tmp/pages_low.dat"

#else
    #define D_FACTOR       0.85
    #define MAX_ITER       1000
    #define THRESHOLD      0.00001
    #define NUM_PAGES      12000
    #define DIVISOR        2
    #define PAGE_FILE_PATH "/tmp/pages_medium.dat"
#endif
