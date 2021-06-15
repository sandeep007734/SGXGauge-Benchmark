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



int main(int ac, char *av[]){

	int parallel = 1;
    int warmup = 0;
    int repetitions = TRIES;
    int c;
    char* usage = "[-P <parallelism>] [-W <warmup>] [-N <repetitions>]\n";

	// #ifndef HAVE_int64
 //    printf("\tAISHAPRINT FILE: %s LINE:%d "
	// 						"HAVE_int64=0\n",
 //                            __FILE__, __LINE__);
 //    #endif

	// #ifdef HAVE_int64
 //    printf("\tAISHAPRINT FILE: %s LINE:%d "
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