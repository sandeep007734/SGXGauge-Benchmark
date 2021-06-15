#include <stdio.h>
#include "drand48.h"

int main(void)
{
	int i = 0;
	for(i = 0; i < 100; i++) {
		double t = drand48();
		printf("%lf\n",t);
	}
}
