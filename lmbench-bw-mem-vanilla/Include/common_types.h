#ifndef COMMON_TYPES_H__
#define COMMON_TYPES_H__
/*
 * Aisha Hasan
 *
 * User types header.
 *
 */

#define	SGX_SUCCESS	1

struct mytimeval
  {
  	long tv_sec;
  	long tv_usec;
  };


#define TYPE    int

typedef struct _state {
    double  overhead;
    size_t  nbytes;
    int need_buf2;
    int aligned;
    TYPE    *buf;
    TYPE    *buf2;
    TYPE    *buf2_orig;
    TYPE    *lastone;
    size_t  N;
} state_t;

typedef unsigned long iter_t;

#endif
