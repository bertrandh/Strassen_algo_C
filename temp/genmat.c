#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define seed 1234567890

int main(int argc, char * argv[])
{
	if (argc <= 4){
		printf("Usage: %s type min max times [seed]\n", argv[0]);
		exit(1);
	} else {
		char type = argv[1][0];
		long min = strtol(argv[2], NULL, 10);
		long max = strtol(argv[3], NULL, 10);
		long times = strtol(argv[4], NULL, 10);
		long s;
		float fact = ((float)(max - min))/RAND_MAX;
		int n, count = 1;

		if (argc >= 6)
			s = strtol(argv[5], NULL, 10);
		else
			s = seed;
		srand(s);
		n = sqrt(times/2);
		while (times-- > 0) {
			switch (type){
			  case 'f':
				  printf("%5f ", rand() * fact + min);
				  break;
			  case 'z':
				  printf("%5ld ", (long)(rand() * fact + min + 0.5));
				  break;
			}
			if (count % n == 0)
				printf("\n");
			if (count++ % (n * n) == 0)
				printf("\n");
		}
		return(0);
	}
}
