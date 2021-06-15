/* 
 * |Aisha Hasan|
 * 
 * The |App/EnclaveBridge.c| file.
 * 
 * 
 * 
 * 
 */

#include "EnclaveBridge.h"

#include <unistd.h> //getppid
#include <sys/stat.h>
#include <fcntl.h>  //open

/* ########## START OCALL functions ########### */
void ocall_print_string(const char *str){
	printf("%s\n", str);
}


void ocall_gettimeofday(struct mytimeval *tv){

	struct timeval temptv;

	//NOTE - doing this because casting not always guaranteed to work
	// (void) gettimeofday((struct timeval *) tv, (struct timezone *) 0); 
	(void) gettimeofday( &temptv, (struct timezone *) 0);
	tv->tv_sec = temptv.tv_sec;
	tv->tv_usec = temptv.tv_usec; 

}


void ocall_do_getppid(iter_t iterations, void *cookie, size_t clen)
{   
    struct _state *pState = (struct _state*)cookie;
    char    c;

    while (iterations-- > 0) {
        getppid();
    }
}


void ocall_do_write(iter_t iterations, void *cookie, size_t clen)
{
    struct _state *pState = (struct _state*)cookie;
    char    c;


    while (iterations-- > 0) {
        if (write(pState->fd, &c, 1) != 1) {
            perror("/dev/null");
            return;
        }
    }
}


void ocall_do_read(iter_t iterations, void *cookie, size_t clen)
{
    struct _state *pState = (struct _state*)cookie;
    char    c;

    while (iterations-- > 0) {
        if (read(pState->fd, &c, 1) != 1) {
            perror("/dev/zero");
            return;
        }
    }
}

void ocall_do_stat(iter_t iterations, void *cookie, size_t clen)
{
    struct _state *pState = (struct _state*)cookie;
    struct  stat sbuf;

    while (iterations-- > 0) {
        if (stat(pState->file, &sbuf) == -1) {
            perror(pState->file);
            return;
        }
    }
}


void ocall_do_fstat(iter_t iterations, void *cookie, size_t clen)
{
    struct _state *pState = (struct _state*)cookie;
    struct  stat sbuf;

    while (iterations-- > 0) {
        if (fstat(pState->fd, &sbuf) == -1) {
            perror("fstat");
            return;
        }
    }
}

void ocall_do_openclose(iter_t iterations, void *cookie, size_t clen)
{
    struct _state *pState = (struct _state*)cookie;
    int fd;

    while (iterations-- > 0) {
        fd = open(pState->file, 0);
        if (fd == -1) {
            perror(pState->file);
            return;
        }
        close(fd);
    }
}


/* ########## END OCALL functions ############ */



void send_params(int parallel, int warmup, int repetitions,
                char *benchmark, size_t blen,
                struct _state cookie){


	int rv = NL_STATUS_OK;

	rv = ecall_initialise(); //initialise an empty enclave

	printf("\tFILE: %s LINE:%d created enclaveID: [%lu].\n",
        __FILE__, __LINE__, enclaveID);


 	ecall_send_params(
                    parallel,
                    warmup,
                    repetitions,
                    benchmark,
                    blen,
                    cookie);


}



int ecall_initialise(){
	int rv = NL_STATUS_OK;

	if(!get_enclave(NULL)) rv = NL_STATUS_SGXERROR;

	return rv;
}


int get_enclave(long unsigned int *eid){
	if(launched) return launched;
	else return create_enclave(NULL);
}


int create_enclave(long unsigned int *eid){
	/* call sgx_create_enclave to initialise an enclave instance */
  	/* sgx_status_t sgx_create_enclave(
		const char *file_name, const int debug, sgx_launch_token_t *launch_token, int *launch_token_updated,
		sgx_enclave_id_t *enclave_id, sgx_misc_attribute_t *misc_attr); */

	// sgx_status = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &enclaveID, NULL);
	sgx_status = SGX_SUCCESS;
	updated = 1;
	enclaveID = 2;

	if(sgx_status == SGX_SUCCESS){
		if(eid != NULL) *eid = enclaveID;
		launched = LAUNCHED;
		return 1;
	}

	printf("FILE %s LINE: %d ERROR %d, failed to create enclave.\n", __FILE__, __LINE__, sgx_status);
	return 0;
}
