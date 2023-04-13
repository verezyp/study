#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int solution = 0;
int F1(FILE* f2, int A1[24][24], int N, int L, int k) {
	int k1 = k;
	int A2[24][24];
	int i1 = 0, j1 = 0;
	memcpy(A2, A1, 24 * 24 * sizeof(int));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (A1[i][j] == 0) {
				i1 = i; 
				j1 = j;
				A1[i][j] = 1;
				for (int h = 0; h < N; h++) {
					if (A1[i][h] != 1) {
						A1[i][h] = 2;
					}
					if (A1[h][j] != 1) {
						A1[h][j] = 2;
					}
				}
				k1 += 1;
				break;
			}
		}
		if (k1 > k) {
			break;
		}
	}
	if (k1 == L) {
		for (int i3 = 0; i3 < N; i3++) {
			for (int j3 = 0; j3 < N; j3++) {
				if (A1[i3][j3] == 1) {
					fprintf(f2, "(%d,%d) ", i3, j3);
					solution = 1;
				}
			}
		}
		fprintf(f2, "\n");
	}
	unsigned short int f = 0;
	for (int i4 = 0; i4 < N; i4++) {
		for (int j4 = 0; j4 < N; j4++) {
			if (A2[i4][j4] == 0) {
				f = 1;
			}
		}
	}
	if (f == 0) {
		return 0;
	}
	if (k1 < L) {
		F1(f2, A1, N, L, k1);
	}	
	if (k1 > k) {
		A2[i1][j1] = 2;
	}
	F1(f2, A2, N, L, k);
}
int main() {
	FILE* f1;
	FILE* f2; 
	f1 = fopen("input.txt", "r");
	f2 = fopen("output.txt", "w");
	int A[24][24];
	int x, y;
	int k = 0;
	memset(A, 0, 24 * 24 * sizeof(int));
	int N = 0,L = 0,K = 0;
	fscanf(f1,"%d%d%d", &N,&L,&K);
	for (int i = 0; i < K; i++) {
		fscanf(f1, "%d%d", &x, &y);
		A[x][y] = 1;
		for (int h = 0; h < N; h++) {
			if (A[x][h] != 1) {
				A[x][h] = 2;
			}
			if (A[h][y] != 1) {
				A[h][y] = 2;
			}
		}
	}
	F1(f2, A, N, L, k);
	if (solution == 0) {
		fprintf(f2,"no solutions");
	}
}
