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




void ecall_send_params(int parallel, int warmup, int repetitions,
                    char *benchmark, size_t blen,
					state_t cookie);
//                    struct _state cookie);


#endif /* !_ENCLAVE_H_ */
