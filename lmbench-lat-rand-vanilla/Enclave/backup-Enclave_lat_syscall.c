/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave.c| file.
 *
 *
 *
 */


#include "Enclave.h"



// void ecall_print_string(const char *str){
// 	wrapper_ocall_print_string(str);
// }

void ecall_send_params(int parallel, int warmup, int repetitions){

	char buf[BUFSIZE] = {'\0'};

	snprintf(buf, BUFSIZE, "\tFILE: %s LINE:%d "
							"parallel: %d, warmup: %d, repetitions: %d.",
                            __FILE__, __LINE__, parallel, warmup, repetitions);
    ocall_print_string(buf);
    // wrapper_ocall_print_string(buf);
    memset(buf, '\0', BUFSIZE);






    benchmp(NULL, bench_drand48, NULL,
		0, parallel, warmup, repetitions, NULL);
	nano("drand48 latency", get_n());

	

}




void bench_drand48(register iter_t iterations, void *cookie)
{
// 	// char buf[BUFSIZE] = {'\0'};

// 	// snprintf(buf, BUFSIZE, "\t%s:%d Inside bench_drand48",
//  //                            __FILE__, __LINE__);
//  //    ocall_print_string(buf);
//  //    memset(buf, '\0', BUFSIZE);

	register double v = 0.0;
	while (iterations-- > 0) {
		v += drand48();
	}
	use_int((int)v);
}

