/*
 * |Aisha Hasan|
 * 
 * The |App/lat_syscall.c| file. 
 *
 */


#include "EnclaveBridge.h"
#include "lat_syscall.h"
#include "bench_app.h"

#include <fcntl.h>
//#define FNAME "/usr/include/x86_64-linux-gnu/sys/types.h"
#define FNAME "empty.txt"

int main(int ac, char *av[]){

	int parallel = 1;
    int warmup = 0;
    int repetitions = TRIES;
    int c;

	struct _state state;

	char* usage = "[-P <parallelism>] [-W <warmup>] [-N <repetitions>] null|read|write|stat|fstat|open [file]\n";



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


	if (optind != ac - 1 && optind != ac - 2 ) {
        lmbench_usage(ac, av, usage);
    }

 	state.file = FNAME;
    if (optind == ac - 2)
        state.file = av[optind + 1];

 	if (streq(av[optind], "null")){
 		send_params(parallel,
					warmup,
					repetitions,
					av[optind],
					strlen(av[optind]) + 1,
					state);
	}
	else if (streq(av[optind], "write")){
	
		state.fd = open("/dev/null", 1);

		printf("\tAISHAPRINT FILE: %s LINE:%d do_write file_des[%d]\n",
			 __FILE__, __LINE__, state.fd);

		send_params(parallel,
                    warmup,
                    repetitions,
                    av[optind],
                    strlen(av[optind]) + 1,
                    state);

		close(state.fd);
	}
	else if (!strcmp("read", av[optind])) {

		state.fd = open("/dev/zero", 0);
		if (state.fd == -1) {
			fprintf(stderr, "Simple read: -1\n");
			return(1);
		}

		send_params(parallel,
                    warmup,
                    repetitions,
                    av[optind],
                    strlen(av[optind]) + 1,
                    state);

		close(state.fd);

	}
	else if (!strcmp("stat", av[optind])) {

		send_params(parallel,
                    warmup,
                    repetitions,
                    av[optind],
                    strlen(av[optind]) + 1,
                    state);

	}
	else if (!strcmp("fstat", av[optind])) {

		printf("\t%s LINE:%d fstat filename[%s]\n",
                         __FILE__, __LINE__, state.file);


		state.fd = open(state.file, 0);
		
		send_params(parallel,
                    warmup,
                    repetitions,
                    av[optind],
                    strlen(av[optind]) + 1,
                    state);
               
		close(state.fd);
	}
	else if (!strcmp("open", av[optind])) {

		printf("\t%s LINE:%d open filename[%s]\n",
                         __FILE__, __LINE__, state.file);

		send_params(parallel,
                    warmup,
                    repetitions,
                    av[optind],
                    strlen(av[optind]) + 1,
                    state);
	}
	else {
		lmbench_usage(ac, av, usage);
	}

	printf("Info: SampleEnclave successfully returned.\n");
	return (0);
}


void
lmbench_usage(int argc, char *argv[], char* usage)
{
	fprintf(stderr,"Usage: %s %s", argv[0], usage);
	exit(-1);
}
