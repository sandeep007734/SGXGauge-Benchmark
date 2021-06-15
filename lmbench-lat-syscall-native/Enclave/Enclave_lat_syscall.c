/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave.c| file.
 *
 *
 *
 */


#include "Enclave_lat_syscall.h"


void ecall_print_string(const char *str){
	ocall_print_string(str);
}

void ecall_send_params(int parallel, int warmup, int repetitions,
					char *benchmark, size_t blen,
					struct _state cookie){
//					void *cookie, size_t clen){



	char buf[BUFSIZE] = {'\0'};

	snprintf(buf, BUFSIZE, "\tAISHAPRINT FILE: %s LINE:%d "
						"parallel: %d, warmup: %d, reps: %d, file_des:[%d].",
                        __FILE__, __LINE__, parallel, warmup, repetitions,
						cookie.fd	);
    ocall_print_string(buf);


    memset(buf, '\0', BUFSIZE);





 	if (!strcmp("null", benchmark)) {
        benchmp(NULL, bench_do_getppid, NULL, 0, parallel,
            warmup, repetitions, &cookie);
        micro("Simple syscall", get_n());
    } 
	else if (!strcmp("write", benchmark)) {

		benchmp(NULL, bench_do_write, NULL, 0, parallel, 
			warmup, repetitions, &cookie);
		micro("Simple write", get_n());
    }

	else if (!strcmp("read", benchmark)) {
		
		benchmp(NULL, bench_do_read, NULL, 0, parallel, 
			warmup, repetitions, &cookie);
		micro("Simple read", get_n());
	}
	else if (!strcmp("stat", benchmark)) {

		benchmp(NULL, bench_do_stat, NULL, 0, parallel, 
			warmup, repetitions, &cookie);
		micro("Simple stat", get_n());
	}  
	else if (!strcmp("fstat", benchmark)) {
	
		benchmp(NULL, bench_do_fstat, NULL, 0, parallel, 
			warmup, repetitions, &cookie);
		micro("Simple fstat", get_n());
	} 
	else if (!strcmp("open", benchmark)) {
		benchmp(NULL, bench_do_openclose, NULL, 0, parallel, 
			warmup, repetitions, &cookie);
		micro("Simple open/close", get_n());
	} 

}




void bench_do_getppid(register iter_t iterations, void *cookie)
{
	ocall_do_getppid(iterations, cookie, sizeof(struct _state));
}


void bench_do_write(register iter_t iterations, void *cookie)
{

/*	char buf[BUFSIZE] = {'\0'};
    snprintf(buf, BUFSIZE, "\tAISHAPRINT FILE: %s LINE:%d file_des:[%d].",
                        __FILE__, __LINE__, tempState->fd);
    ocall_print_string(buf);
    memset(buf, '\0', BUFSIZE);

*/
    ocall_do_write(iterations, cookie, sizeof(struct _state));
}


void bench_do_read(register iter_t iterations, void *cookie)
{
    ocall_do_read(iterations, cookie, sizeof(struct _state));
}


void bench_do_stat(register iter_t iterations, void *cookie)
{
    ocall_do_stat(iterations, cookie, sizeof(struct _state));
}


void bench_do_fstat(register iter_t iterations, void *cookie)
{
    ocall_do_fstat(iterations, cookie, sizeof(struct _state));
}


void bench_do_openclose(register iter_t iterations, void *cookie)
{
    ocall_do_openclose(iterations, cookie, sizeof(struct _state));
}

