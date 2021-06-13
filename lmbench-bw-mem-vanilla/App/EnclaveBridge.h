/* 
 * |Aisha Hasan|
 * 
 * The |App/EnclaveBridge.h| file.
 * 
 * 
 * 
 * 
 */
#ifndef _ENCLAVE_BRIDGE_H_
#define _ENCLAVE_BRIDGE_H_


// #include "Enclave_u.h"
#include	"../Enclave/Enclave_bw_mem.h"
// #include	"common_types.h"

#include <stdio.h>
#include <sys/time.h>

// #include "sgx_error.h"	/* sgx_status_t */
// #include "sgx_eid.h"  	/* sgx_enclave_id_t */
// #include "sgx_urts.h"	/* sgx_launch_token_t */
#include "LMBenchError.h"





#define NOT_LAUNCHED 0
#define LAUNCHED 1
#define ENCLAVE_FILENAME "enclave.signed.so"


// static sgx_enclave_id_t enclaveID = 4;
static long unsigned int enclaveID = 4;
static int launched = NOT_LAUNCHED;
static int updated = 0;
// static sgx_launch_token_t token = {0};
static int token = {0};
// static sgx_status_t sgx_status= SGX_SUCCESS;
static int sgx_status = NL_STATUS_OK;


void ocall_print_string(const char* str);
void ocall_gettimeofday(struct mytimeval *tv);

//void send_params(int parallel, int warmup, int repetitions);
void send_params(int parallel, int warmup, int repetitions,
                char *benchmark, size_t blen,
                struct _state cookie);

int ecall_initialise(void);
int get_enclave(long unsigned int *eid);
int create_enclave(long unsigned int *eid);


#endif /* !_ENCLAVE_BRIDGE_H_ */
