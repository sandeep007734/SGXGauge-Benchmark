#ifdef HIGH_
    #define D_FACTOR       0.85
    #define MAX_ITER       1000
    #define THRESHOLD      0.00001
    #define NUM_PAGES      5000
    #define DIVISOR        2
    #define PAGE_FILE_PATH "pages_high.dat"

#elif LOW_
    #define D_FACTOR       0.85
    #define MAX_ITER       1000
    #define THRESHOLD      0.00001
    #define NUM_PAGES      4500
    #define DIVISOR        2
    #define PAGE_FILE_PATH "pages_low.dat"

#else
    #define D_FACTOR       0.85
    #define MAX_ITER       1000
    #define THRESHOLD      0.00001
    #define NUM_PAGES      4750
    #define DIVISOR        2
    #define PAGE_FILE_PATH "pages_medium.dat"
#endif
