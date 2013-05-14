#include <stdio.h>
#include <stdlib.h>

#define N0 16 	// Best compromise between the cross-over number found experimentally 
				//for powers of 2 up to n = 1024 (and equal to 31) and the one found
				//by computation (and equal to 13)

int strassmult(float **, int, int, float **, int, int, float **, int, int, int);
int regmult(int, float **, int, int, int, int, float **, int, int, int, int, float **, int, int);
void display(float **, int, int, int);
