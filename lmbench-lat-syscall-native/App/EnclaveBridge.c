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

#include <unistd.h>	//getppid
#include <sys/stat.h>
#include <fcntl.h>	//open

// #define	_DEBUG_RAND

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

	// #ifdef _DEBUG_RAND
	// printf("\tAISHAPRINT FILE: %s LINE:%d\n"
	// 		"\tsecs=%lu"
	// 		"\tusecs=%lu\n",
	// 		__FILE__, __LINE__, tv->tv_sec, tv->tv_usec);	
	// #endif
}


void ocall_do_getppid(iter_t iterations, void *cookie, size_t clen)
{
	struct _state *pState = (struct _state*)cookie;
	char	c;

	while (iterations-- > 0) {
		getppid();
	}
}


void ocall_do_write(iter_t iterations, void *cookie, size_t clen)
{
	struct _state *pState = (struct _state*)cookie;
	char	c;


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
	char	c;

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
	struct	stat sbuf;

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
	struct	stat sbuf;

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
	int	fd;

	while (iterations-- > 0) {

		fd = open(pState->file, 0);

		printf("\t%s LINE:%d open filede[%d]\n",
                        __FILE__, __LINE__, fd);

		
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
//				void *cookie, size_t clen){	


	int rv = NL_STATUS_OK;

	rv = ecall_initialise(); //initialise an empty enclave

	printf("AISHAPRINT FILE: %s LINE:%d created enclaveID: [%lu].\n",
        __FILE__, __LINE__, enclaveID);

	ecall_print_string(enclaveID, "987");

	ecall_send_params(enclaveID,
					parallel,
					warmup,
					repetitions,
					benchmark,
					blen,
					cookie);
//					clen);

	printf("AISHAPRINT FILE: %s LINE:%d destroy enclaveID: [%lu].\n",
        __FILE__, __LINE__, enclaveID);

	printf("AISHAPRINT FILE: %s LINE:%d %s destroyed enclaveID: [%lu].\n",
		__FILE__, __LINE__,
		(sgx_destroy_enclave(enclaveID) != SGX_SUCCESS)? "not":"", enclaveID);


}



int ecall_initialise(){
	int rv = NL_STATUS_OK;

	if(!get_enclave(NULL)) rv = NL_STATUS_SGXERROR;

	return rv;
}


int get_enclave(sgx_enclave_id_t *eid){
	if(launched) return launched;
	else return create_enclave(NULL);
}


int create_enclave(sgx_enclave_id_t *eid){
	/* call sgx_create_enclave to initialise an enclave instance */
  	/* sgx_status_t sgx_create_enclave(
		const char *file_name, const int debug, sgx_launch_token_t *launch_token, int *launch_token_updated,
		sgx_enclave_id_t *enclave_id, sgx_misc_attribute_t *misc_attr); */

	sgx_status = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &enclaveID, NULL);


	if(sgx_status == SGX_SUCCESS){
		if(eid != NULL) *eid = enclaveID;
		launched = LAUNCHED;
		return 1;
	}

	printf("AISHAPRINT FILE %s LINE: %d ERROR %d, failed to create enclave.\n", __FILE__, __LINE__, sgx_status);
	return 0;
}
