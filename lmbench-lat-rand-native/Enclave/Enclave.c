/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave.c| file.
 *
 *
 *
 */


#include "Enclave.h"


void ecall_print_string(const char *str){
	ocall_print_string(str);
}

void ecall_send_params(int parallel, int warmup, int repetitions){

	char buf[BUFSIZE] = {'\0'};

	snprintf(buf, BUFSIZE, "\tAISHAPRINT FILE: %s LINE:%d "
							"parallel: %d, warmup: %d, repetitions: %d.",
                            __FILE__, __LINE__, parallel, warmup, repetitions);
    ocall_print_string(buf);
    memset(buf, '\0', BUFSIZE);

    benchmp(NULL, bench_drand48, NULL,
		0, parallel, warmup, repetitions, NULL);
	nano("drand48 latency", get_n());

	
	benchmp(NULL, bench_lrand48, NULL,
		0, parallel, warmup, repetitions, NULL);
	nano("lrand48 latency", get_n());

}




void bench_drand48(register iter_t iterations, void *cookie)
{
	// char buf[BUFSIZE] = {'\0'};

	// snprintf(buf, BUFSIZE, "\t%s:%d Inside bench_drand48",
 //                            __FILE__, __LINE__);
 //    ocall_print_string(buf);
 //    memset(buf, '\0', BUFSIZE);

	register double v = 0.0;
	while (iterations-- > 0) {
		v += drand48();
	}
	use_int((int)v);


	// snprintf(buf, BUFSIZE, "FILE %s:%d SUM: [%f], iter: %lu.\n",
	// 					__FILE__, __LINE__, v, iterations);
 //    ocall_print_string(buf);
 //    memset(buf, '\0', BUFSIZE);
}

void bench_lrand48(register iter_t iterations, void *cookie)
{
	// register unsigned int r;


	// char buf[BUFSIZE] = {'\0'};
	// snprintf(buf, BUFSIZE, "\t%s:%d Inside bench_lrand48",
 //                            __FILE__, __LINE__);
 //    ocall_print_string(buf);
 //    memset(buf, '\0', BUFSIZE);


	register long v = 0.0;
	while (iterations-- > 0) {
		v += lrand48();
		// sgx_read_rand((unsigned char *) &r, sizeof(unsigned int));
		// v += r;
	}
	use_int((int)v);


	// snprintf(buf, BUFSIZE, "%s:%d SUM: [%lu] RAND: [%d], iter: %lu.\n",
	// 					__FILE__, __LINE__, v, r, iterations);
 //    ocall_print_string(buf);
 //    memset(buf, '\0', BUFSIZE);


}
