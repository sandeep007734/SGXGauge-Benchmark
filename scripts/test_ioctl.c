#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
#include <stdint.h>

struct sgxgauge_counters{


	unsigned int sgx_nr_epc_freepages;
	unsigned int sgx_nr_epc_evictions;

	unsigned int sgx_nr_epc_eldu;
	unsigned int nr_sgx_cpu_flushes;
	unsigned int nr_sgx_cpus_flushes;

	unsigned int nr_sgx_sec_was_evicted;
	unsigned int nr_sgx_eldu_load;

};

#define WR_VALUE _IOW('a','a',uint64_t*)
#define RD_VALUE _IOR('a','b',struct sgxgauge_counters*)
 

struct sgxgauge_counters sgxgauge_counters_i;

void sgxgauge_print_info(void){
        printf("sgx_nr_epc_freepages: %d\n", sgxgauge_counters_i.sgx_nr_epc_freepages);
        printf("sgx_nr_epc_evictions: %d\n", sgxgauge_counters_i.sgx_nr_epc_evictions);
        printf("nr_sgx_cpu_flushes: %d\n", sgxgauge_counters_i.nr_sgx_cpu_flushes);
        printf("nr_sgx_cpus_flushes: %d\n", sgxgauge_counters_i.nr_sgx_cpus_flushes);
        printf("nr_sgx_sec_was_evicted: %d\n", sgxgauge_counters_i.nr_sgx_sec_was_evicted);
        printf("nr_sgx_eldu_load: %d\n", sgxgauge_counters_i.nr_sgx_eldu_load);
}

int main(int argc, char *argv[])
{
        int fd;
        int value, number;
        

        printf("SGXGauge Counters. Mode %d\n",argc);
        // printf("\nOpening Driver\n");

        fd = open("/dev/isgx", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
        
        if (argc > 1){
                number=1;
                ioctl(fd, WR_VALUE, (uint64_t*) &number); 
        }
        else{
                ioctl(fd, RD_VALUE, (struct sgxgauge_counters*) &sgxgauge_counters_i);
                sgxgauge_print_info();
        }

        close(fd);
        
}