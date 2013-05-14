#include "strassm.h"
#include <stdio.h>

static void set(float **, int, int, float **, int, int, int);
static int addsub(int, float **, int, int, float **, int, int, float **, int, int, int);
static int splitmult(float **, int, int, float **, int, int, float **, int, int, int, int);

extern int n0;

int strassmult(float ** m1, int i1, int j1, float ** m2, int i2, int j2, float ** m3, int i3, int j3, int n)
{
	long long c = 0;
	if (n <= n0) {
		c += regmult(0, m1, i1, j1, n, n, m2, i2, j2, n, n, m3, i3, j3);
	} else {
		if (n % 2 == 1){
			c += splitmult(m1, i1, j1, m2, i2, j2, m3, i3, j3, n, n-1);
		} else {
			// Make the submatrices start index clearer
			int nn = n/2;
			int iA = i1, jA = j1, iB = i1, jB = (j1 + nn); 
			int iC = (i1 + nn), jC = j1, iD = iC, jD = jB;
			int iE = i2, jE = j2, iF = i2, jF = (j2 + nn);
			int iG = (i2 + nn), jG = j2, iH = iG, jH = jF;
			int iK = i3, jK = j3, iL = i3, jL = (j3 + nn);
			int iM = (i3 + nn), jM = j3, iN = iM, jN = jL;
			c += 7;
			// L = (A + D) and M = (E + H) and N = L * M = P5
			c += addsub(+1, m1, iA, jA, m1, iD, jD, m3, iL, jL, nn);
			c += addsub(+1, m2, iE, jE, m2, iH, jH, m3, iM, jM, nn);
			c += strassmult(m3, iL, jL, m3, iM, jM, m3, iN, jN, nn);
			// L = (B - D) and M = (G + H) and K = L * M = P6
			c += addsub(-1, m1, iB, jB, m1, iD, jD, m3, iL, jL, nn);
			c += addsub(+1, m2, iG, jG, m2, iH, jH, m3, iM, jM, nn);
			c += strassmult(m3, iL, jL, m3, iM, jM, m3, iK, jK, nn);
			// M = (A + B)
			c += addsub(+1, m1, iA, jA, m1, iB, jB, m3, iM, jM, nn);
			// Now B does not appear in any later formula so we can reuse it
			// set B = H and L = M * B = P2
			set(m1, iB, jB, m2, iH, jH, nn);
			c += strassmult(m3, iM, jM, m1, iB, jB, m3, iL, jL, nn);
			// B = (G - E)
			c += addsub(-1, m2, iG, jG, m2, iE, jE, m1, iB, jB, nn);
			// Now G does not appear in any later formula so we can reuse it
			// set G = D and M = G * B = P4
			set(m2, iG, jG, m1, iD, jD, nn);
			c += strassmult(m2, iG, jG, m1, iB, jB, m3, iM, jM, nn);
			// B = (C + D)
			c += addsub(+1, m1, iC, jC, m1, iD, jD, m1, iB, jB, nn);
			// Now D does not appear in any later formula so we can reuse it
			// set D = E and G = B * D = P3
			set(m1, iD, jD, m2, iE, jE, nn);
			c += strassmult(m1, iB, jB, m1, iD, jD, m2, iG, jG, nn);
			// B = (F - H)
			c += addsub(-1, m2, iF, jF, m2, iH, jH, m1, iB, jB, nn);
			// Now H does not appear in the last formula (for P7) so we reuse it
			// set D = A and H = D * B = P1
			set(m1, iD, jD, m1, iA, jA, nn);
			c += strassmult(m1, iD, jD, m1, iB, jB, m2, iH, jH, nn);
			// B = (C - A) and D = (E + F)
			// and reuse E for E = B * D = P7
			c += addsub(-1, m1, iC, jC, m1, iA, jA, m1, iB, jB, nn);
			c += addsub(+1, m2, iE, jE, m2, iF, jF, m1, iD, jD, nn);
			c += strassmult(m1, iB, jB, m1, iD, jD, m2, iE, jE, nn);
			// Now finish filling K, L, M, N:
			// K = K + N + M - L = P6 + P5 + P4 - P2
			c += addsub(+1, m3, iK, jK, m3, iN, jN, m3, iK, jK, nn);
			c += addsub(+1, m3, iK, jK, m3, iM, jM, m3, iK, jK, nn);
			c += addsub(-1, m3, iK, jK, m3, iL, jL, m3, iK, jK, nn);
			// L = L + H = P2 + P1
			c += addsub(+1, m3, iL, jL, m2, iH, jH, m3, iL, jL, nn);
			// M = M + G = P4 + P3
			c += addsub(+1, m3, iM, jM, m2, iG, jG, m3, iM, jM, nn);
			// N = N + H + F - G = P4 + P1 + P7 - P3
			c += addsub(+1, m3, iN, jN, m2, iH, jH, m3, iN, jN, nn);
			c += addsub(+1, m3, iN, jN, m2, iE, jE, m3, iN, jN, nn);
			c += addsub(-1, m3, iN, jN, m2, iG, jG, m3, iN, jN, nn);
		}
	}
	return(c);
}

int regmult(int add, float ** m1, int i1, int j1, int r1, int c1, float ** m2, int i2, int j2, int r2, int c2, float ** m3, int i3, int j3)
{
	int i,j,k;
	long long c = 0;
	if (c1 != r2) {
		fprintf(stderr, "number of coloumn %d of first matrix does not match number of rows %d of second matrix\n", c1, r2);
		exit(1);
	}
	for (i = 0; i < r1; i++)
		for (j = 0; j < c2; j++) {
			if (add == 0)
				m3[i + i3][j + j3] = 0;
			for (k = 0; k < c1; k++) {
				m3[i + i3][j + j3] += m1[i + i1][k + j1] * m2[k + i2][j + j2];
				c += 2;
			}
		}
	return(c);
}

int addsub(int sign, float ** m1, int i1, int j1, float ** m2, int i2, int j2, float ** m3, int i3, int j3, int n)
{
	int i,j;
	long long c = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++){
			m3[i3 + i][j3 + j] = m1[i1 + i][j1 + j] + sign * m2[i2 + i][j2 + j];
			c++;
		}
	return(c);
}

int splitmult(float ** m1, int i1, int j1, float ** m2, int i2, int j2, float ** m3, int i3, int j3, int n, int s)
{
	long long c = 0;
	int r = n - s;
	int iA = i1, jA = j1, iB = i1, jB = (j1 + s); 
	int iC = (i1 + s), jC = j1, iD = iC, jD = jB;
	int iE = i2, jE = j2, iF = i2, jF = (j2 + s);
	int iG = (i2 + s), jG = j2, iH = iG, jH = jF;
	int iK = i3, jK = j3, iL = i3, jL = (j3 + s);
	int iM = (i3 + s), jM = j3, iN = iM, jN = jL;

	c += 7;
	// L = (A * F)
	c += regmult(0, m1, iA, jA, s, s, m2, iF, jF, s, r, m3, iL, jL);
	// L = L + (B * H)
	c += regmult(1, m1, iB, jB, s, r, m2, iH, jH, r, r, m3, iL, jL);

	// M = (C * E)
	c += regmult(0, m1, iC, jC, r, s, m2, iE, jE, s, s, m3, iM, jM);
	// M = M + (D * G)
	c += regmult(1, m1, iD, jD, r, r, m2, iG, jG, r, s, m3, iM, jM);

	// K = (A * E);  strassmult(A,E) messes up A and E so it has to be 
	// after other uses of A and E
	c += strassmult(m1, iA, jA, m2, iE, jE, m3, iK, jK, s);
	// K = K + (B * G)
	c += regmult(1, m1, iB, jB, s, r, m2, iG, jG, r, s, m3, iK, jK);
	
	// N = (D * H)
	c += strassmult(m1, iD, jD, m2, iH, jH, m3, iN, jN, r);
	// N = N + (C * F)
	c += regmult(1, m1, iC, jC, r, s, m2, iF, jF, s, r, m3, iN, jN);

	return(c);
}

void set(float ** m1, int i1, int j1, float ** m2, int i2, int j2, int n)
{
	int i,j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			m1[i1 + i][j1 + j] = m2[i2 + i][j2 + j];
}
