/*
 * |Aisha Hasan|
 * 
 * Testing an empty enclave.
 * The program will enter the enclave, print, and exit.
 *
 */


// #include "bench.h"
#include "EnclaveBridge.h"
#include "lat_rand.h"
#include "bench_app.h"


// #define	_DEBUG_RAND

/* ########## START OCALL functions ########### */
// void ocall_print_string(const char *str){
// 	printf("%s\n", str);
// }


// void ocall_gettimeofday(struct mytimeval *tv){

// 	struct timeval temptv;

// 	//NOTE - doing this because casting not always guaranteed to work
// 	// (void) gettimeofday((struct timeval *) tv, (struct timezone *) 0); 
// 	(void) gettimeofday( &temptv, (struct timezone *) 0);
// 	tv->tv_sec = temptv.tv_sec;
// 	tv->tv_usec = temptv.tv_usec; 

// 	// #ifdef _DEBUG_RAND
// 	// printf("\tFILE: %s LINE:%d\n"
// 	// 		"\tsecs=%lu"
// 	// 		"\tusecs=%lu\n",
// 	// 		__FILE__, __LINE__, tv->tv_sec, tv->tv_usec);	
// 	// #endif
// }
/* ########## END OCALL functions ############ */



int main(int ac, char *av[]){

	int parallel = 1;
    int warmup = 0;
    int repetitions = TRIES;
    int c;
    char* usage = "[-P <parallelism>] [-W <warmup>] [-N <repetitions>]\n";

	// #ifndef HAVE_int64
 //    printf("\tFILE: %s LINE:%d "
	// 						"HAVE_int64=0\n",
 //                            __FILE__, __LINE__);
 //    #endif

	// #ifdef HAVE_int64
 //    printf("\tFILE: %s LINE:%d "
	// 						"HAVE_int64=1\n",
 //                            __FILE__, __LINE__);
 //    #endif



	while (( c = getopt(ac, av, "P:W:N:")) != EOF) {
        switch(c) {
        case 'P':
            parallel = atoi(optarg);
            if (parallel <= 0) lmbench_usage(ac, av, usage);
            break;
        case 'W':
            warmup = atoi(optarg);
            break;
        case 'N':
            repetitions = atoi(optarg);
            break;
        default:
            lmbench_usage(ac, av, usage);
            break;
        }
    }
    if (optind < ac) {
        lmbench_usage(ac, av, usage);
    }


	send_params(parallel, warmup, repetitions);

	printf("Info: SampleEnclave successfully returned.\n");
	return (0);
}


void
lmbench_usage(int argc, char *argv[], char* usage)
{
	fprintf(stderr,"Usage: %s %s", argv[0], usage);
	exit(-1);
}