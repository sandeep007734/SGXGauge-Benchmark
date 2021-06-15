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


struct _state
{
    int fd;
    char *file;
};

typedef unsigned long iter_t;

#endif
