#include "strassm.h"

void errmess(int, char *, char *);
int n0;

int main(int argc, char * argv[])
{
	if (argc < 4) {
		printf("Usage: %s option dimension textfile\n" \
				"\toption: 0 (display diagonal entries of result from Strassen), \n"\
				"\t\t1 (display full result from Strassen), \n"\
				"\t\t2 (display full result from Regular), \n"\
				"\t\t3 (test to find best crossover point)\n", argv[0]);
		exit(1);
	} else {
		const long n = strtol(argv[2], NULL, 10); 
		int i, j, k;
		long long c, d;
		float ** m1, ** m2, ** m3;
		FILE * fp = fopen(argv[3], "r");
		long a1 = strtol(argv[1], NULL, 10);
		n0 = N0;
		m1 = malloc(n * sizeof(float *));
		m2 = malloc(n * sizeof(float *));
		m3 = malloc(n * sizeof(float *));
		for (i = 0; i < n; i++) {
			m1[i] = malloc(n * sizeof(float));
			m2[i] = malloc(n * sizeof(float));
			m3[i] = malloc(n * sizeof(float));
		}
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++) {
				c = fscanf(fp, "%f", &(m1[i][j]));
				if (c == EOF)
					errmess(1, argv[0], "reached end of file before end of m1");
				m3[i][j] = 0;
			}
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++) {
				c = fscanf(fp, "%f", &(m2[i][j]));
				if (c == EOF)
					errmess(2, argv[0], "reached end of file before end of m2");
			}
		fclose(fp);
		switch (a1) {
		  case 0: 
			  c = strassmult(m1, 0, 0, m2, 0, 0, m3, 0, 0, n); 
			  for (i = 0; i < n; i++) 
				  printf("%5.2f\n", (m3[i][i])); 
			  break;
		  case 1: 
			  c = strassmult(m1, 0, 0, m2, 0, 0, m3, 0, 0, n); 
			  display(m3, 0, 0, n); 
			  printf("number of operations: %lld\n", c);
			  break;
		  case 2: 
			  c = regmult(0, m1, 0, 0, n, n, m2, 0, 0, n, n, m3, 0, 0);
			  display(m3, 0, 0, n); 
			  printf("number of operations: %lld\n", c);
			  break;
		  default: {
			  for (k = 1; k <= n; k*=2){
				  n0 = 2;
				  c = strassmult(m1, 0, 0, m2, 0, 0, m3, 0, 0, k);
				  d = c;
				  while ((c <= d) && (n0 <= k)){
					  d = c;
					  n0++;
					  c = strassmult(m1, 0, 0, m2, 0, 0, m3, 0, 0, k);
				  } 
				  printf("%d %lld\n", n0 - 1, d);
			  } 
			  exit(0);
		  }
		}
		for (i = 0; i < n; i++) {
			free(m1[i]);
			free(m2[i]);
			free(m3[i]);
		}
		free(m1);
		free(m2);
		free(m3);
		return(0);
	}
}

void errmess(int errcode, char * filename, char * mess)
{
	fprintf(stderr, "Error %d in %s: %s\n", errcode, filename, mess);
	exit(errcode);
}

void display(float ** m, int i, int j, int n)
{
	int ii, jj;
	for (ii = 0; ii < n; ii++) {
		for (jj = 0; jj < n; jj++)
			printf("%5d ", (int)(m[i + ii][j + jj]));
		printf("\n");
	}
	printf("\n");
}
	
