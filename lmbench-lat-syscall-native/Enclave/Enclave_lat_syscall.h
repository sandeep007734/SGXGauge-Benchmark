/*
 * Aisha Hasan
 *
 * Header file for |Enclave.c|
 *
 */

#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_


#include "sgx_trts.h"
#include "bench.h"



void ecall_print_string(const char *str);
void ecall_send_params(int parallel, int warmup, int repetitions,
					char *benchmark, size_t blen,
					struct _state cookie);


void bench_do_getppid	(iter_t iterations, void *cookie);
void bench_do_write	(iter_t iterations, void *cookie);
void bench_do_read	(iter_t iterations, void *cookie);
void bench_do_stat	(iter_t iterations, void *cookie);
void bench_do_fstat	(iter_t iterations, void *cookie);
void bench_do_openclose	(iter_t iterations, void *cookie);


#endif /* !_ENCLAVE_H_ */
