/*
 * Aisha Hasan
 *
 * Header file for |Enclave.c|
 *
 */

#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_


// #include "Enclave_t.h"

// #include "sgx_trts.h"
#include "bench.h"
#include "../App/EnclaveBridge.h"

// #include <string.h> 



// void ecall_print_string(const char *str);
void ecall_send_params(int parallel, int warmup, int repetitions);


// void bench_drand48(iter_t iterations);
// void bench_lrand48(iter_t iterations);
void bench_drand48(iter_t iterations, void *cookie);
void bench_lrand48(iter_t iterations, void *cookie);

#endif /* !_ENCLAVE_H_ */