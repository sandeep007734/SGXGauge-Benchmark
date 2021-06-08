#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "common_rand.h"

void stopwatch_start(stopwatch *sw){
    if (sw == NULL)
        return;

    memset(&sw->begin, 0, sizeof(struct timeval));
    memset(&sw->end  , 0, sizeof(struct timeval));

    gettimeofday(&sw->begin, NULL);
}

void stopwatch_stop(stopwatch *sw){
    if (sw == NULL)
        return;

    gettimeofday(&sw->end, NULL);
}

double
get_interval_by_sec(stopwatch *sw){
    if (sw == NULL)
        return 0;
    return ((double)(sw->end.tv_sec-sw->begin.tv_sec)+(double)(sw->end.tv_usec-sw->begin.tv_usec)/1000000);
}

int
get_interval_by_usec(stopwatch *sw){
    if (sw == NULL)
        return 0;
    return ((sw->end.tv_sec-sw->begin.tv_sec)*1000000+(sw->end.tv_usec-sw->begin.tv_usec));
}

double matlab_modulo(double x, double y) {
    double n = floor(x/y);
    return (x - n*y);
}

int fletcher_sum_1d_array(double *a, int size) {
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 0; i < size; ++i) {
        sum1 = matlab_modulo((sum1 + a[i]),255);
        sum2 = matlab_modulo((sum2 + sum1),255);
    } 

    return sum2 * 256 + sum1;
}

int fletcher_sum_1d_array_float(float *a, int size) {
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 0; i < size; ++i) {
        sum1 = matlab_modulo((sum1 + a[i]),255);
        sum2 = matlab_modulo((sum2 + sum1),255);
    }

    return sum2 * 256 + sum1;
}

int fletcher_sum_2d_array(double *a, int nbRow, int nbCol) {
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 0; i < nbCol; ++i) {
        for (int j = 0; j < nbRow; ++j) {
            sum1 = matlab_modulo((sum1 + (a[j*nbCol + i])),255);
            sum2 = matlab_modulo((sum2 + sum1),255);
        }
    }

    return sum2 * 256 + sum1;
}

int fletcher_sum_1d_array_int(int *a, int size) {
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 0; i < size; ++i) {
        sum1 = matlab_modulo((double)(sum1 + (double)a[i]),(double)255);
        sum2 = matlab_modulo((double)(sum2 + sum1),(double)255);
    } 

    return sum2 * 256 + sum1;
}

int fletcher_sum_1d_array_unsigned_int(unsigned int *a, int size) {
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 0; i < size; ++i) {
        sum1 = matlab_modulo((double)(sum1 + (double)a[i]),(double)255);
        sum2 = matlab_modulo((double)(sum2 + sum1),(double)255);
    } 

    return sum2 * 256 + sum1;
}
