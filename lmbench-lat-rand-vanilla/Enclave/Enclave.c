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

    benchmp(NULL, bench_mydrand48, NULL,
		0, parallel, warmup, repetitions, NULL);
	nano("mydrand48 latency", get_n());

	
	benchmp(NULL, bench_mylrand48, NULL,
		0, parallel, warmup, repetitions, NULL);
	nano("mylrand48 latency", get_n());

}




/* ######################## DRAND/LRAND FUNCTIONS ########################## */
#define __glibc_unlikely(cond)        __builtin_expect ((cond), 0)
typedef unsigned long int __uint64_t;
typedef __uint64_t uint64_t;

typedef unsigned int __uint32_t;
typedef __uint32_t uint32_t;

#define IEEE754_DOUBLE_BIAS        0x3ff

# define weak_alias(name, aliasname) _weak_alias (name, aliasname)
# define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name))) \
    __attribute_copy__ (name);

union ieee754_double
  {
    double d;
    /* This is the IEEE 754 double-precision format.  */
    struct
      {
        /* Together these comprise the mantissa.  */
        unsigned int mantissa1:32;
        unsigned int mantissa0:20;
        unsigned int exponent:11;
        unsigned int negative:1;
      } ieee;
    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
      {
        unsigned int mantissa1:32;
        unsigned int mantissa0:19;
        unsigned int quiet_nan:1;
        unsigned int exponent:11;
        unsigned int negative:1;
      } ieee_nan;
  };

struct mydrand48_data
  {
    unsigned short int __x[3];        /* Current state.  */
    unsigned short int __old_x[3]; /* Old state.  */
    unsigned short int __c;        /* Additive const. in congruential formula.  */
    unsigned short int __init;        /* Flag for initializing.  */
    __extension__ unsigned long long int __a;        /* Factor in congruential
                                                   formula.  */
  };

struct mydrand48_data __libc_mydrand48_data;

int
__mydrand48_iterate (unsigned short int xsubi[3], struct mydrand48_data *buffer)
{
  uint64_t X;
  uint64_t result;
  /* Initialize buffer, if not yet done.  */
  if (__glibc_unlikely (!buffer->__init))
    {
      buffer->__a = 0x5deece66dull;
      buffer->__c = 0xb;
      buffer->__init = 1;
    }
  /* Do the real work.  We choose a data type which contains at least
     48 bits.  Because we compute the modulus it does not care how
     many bits really are computed.  */
  X = (uint64_t) xsubi[2] << 32 | (uint32_t) xsubi[1] << 16 | xsubi[0];
  result = X * buffer->__a + buffer->__c;
  xsubi[0] = result & 0xffff;
  xsubi[1] = (result >> 16) & 0xffff;
  xsubi[2] = (result >> 32) & 0xffff;
  return 0;
}

int
__erand48_r (unsigned short int xsubi[3], struct mydrand48_data *buffer,
             double *result)
{
  union ieee754_double temp;
  /* Compute next state.  */
  if (__mydrand48_iterate (xsubi, buffer) < 0)
    return -1;
  /* Construct a positive double with the 48 random bits distributed over
     its fractional part so the resulting FP number is [0.0,1.0).  */
  temp.ieee.negative = 0;
  temp.ieee.exponent = IEEE754_DOUBLE_BIAS;
  temp.ieee.mantissa0 = (xsubi[2] << 4) | (xsubi[1] >> 12);
  temp.ieee.mantissa1 = ((xsubi[1] & 0xfff) << 20) | (xsubi[0] << 4);
  /* Please note the lower 4 bits of mantissa1 are always 0.  */
  *result = temp.d - 1.0;
  return 0;
}
//weak_alias (__erand48_r, erand48_r)

double mydrand48(void)
{
  double result;
  (void) __erand48_r (__libc_mydrand48_data.__x, &__libc_mydrand48_data, &result);
  return result;
}



/**************** LRAND48 stuff *********************/

int
__nrand48_r (unsigned short int xsubi[3], struct mydrand48_data *buffer,
             long int *result)
{
  /* Compute next state.  */
  if (__mydrand48_iterate (xsubi, buffer) < 0)
    return -1;
  /* Store the result.  */
  if (sizeof (unsigned short int) == 2)
    *result = xsubi[2] << 15 | xsubi[1] >> 1;
  else
    *result = xsubi[2] >> 1;
  return 0;
}
// weak_alias (__nrand48_r, nrand48_r)




long int
mylrand48 (void)
{
  long int result;
  (void) __nrand48_r (__libc_mydrand48_data.__x, &__libc_mydrand48_data, &result);
  return result;
}


/* ######################################################################### */


























void bench_mydrand48(register iter_t iterations, void *cookie)
{
// 	// char buf[BUFSIZE] = {'\0'};

// 	// snprintf(buf, BUFSIZE, "\t%s:%d Inside bench_mydrand48",
//  //                            __FILE__, __LINE__);
//  //    ocall_print_string(buf);
//  //    memset(buf, '\0', BUFSIZE);

	register double v = 0.0;
	while (iterations-- > 0) {
		v += mydrand48();
	}
	use_int((int)v);
}

void bench_mylrand48(register iter_t iterations, void *cookie)
{

	

	register long v = 0.0;
	// unsigned int r;
	// register unsigned int r;


	// char buf[BUFSIZE] = {'\0'};

	// snprintf(buf, BUFSIZE, "%s:%d SUM: [%lu] RAND: [%d], iter: %lu.\n",
	// 					__FILE__, __LINE__, v, r, iterations);
 //    ocall_print_string(buf);
 //    memset(buf, '\0', BUFSIZE);



	while (iterations-- > 0) {
		v += mylrand48();
		//sgx_read_rand((unsigned char *) &r, sizeof(unsigned int));
		//v += r;
	}
	use_int((int)v);


	// snprintf(buf, BUFSIZE, "%s:%d SUM: [%lu] RAND: [%d], iter: %lu.\n",
	// 					__FILE__, __LINE__, v, r, iterations);
 //    ocall_print_string(buf);
 //    memset(buf, '\0', BUFSIZE);


}
