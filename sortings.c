#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define SIZE 10
void bubble(int* mass) {
	unsigned long long srav = 0, per = 0;
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	int proverka = 0;
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		printf("Error\n");
		return 0;
	}
	PCFreq = (double)(li.QuadPart);
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	/*long double t1 = clock();*/
	for (int i = 1; i < SIZE; i++) {
		for (int j = 0; j < SIZE - i; j++) {
			srav += 1;
			if (mass[j] > mass[j + 1]) {
				int x = mass[j];
				per += 1;
				mass[j] = mass[j + 1];
				mass[j + 1] = x;
				proverka++;
			}
		}
		if (proverka == 0)
		{
			break;
		}
		proverka = 0;
	}
	QueryPerformanceCounter(&li);
	/*long double t2 = clock();
	long double tt = (t2 - t1) / CLOCKS_PER_SEC;*/
	printf("Stats of the bubble sort:\n");
	printf("- Amount of permutation =  %llu\n", per);
	printf("- Amount of comparisons = %llu\n", srav);
	printf("- Time of work = %.15lf seconds\n", (double)(li.QuadPart - CounterStart)/PCFreq);
	//printf("- Time of work = %.15Lf seconds\n", tt);
	printf("\n");
}	
void shell(int* mass) {
	unsigned long long srav = 0, per = 0;
	/*long double tt = 0;
	long double t1 = clock();*/
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		printf("Error\n");
		return 0;
	}
	PCFreq = (double)(li.QuadPart);
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	int d = SIZE;
	d = d / 2;
	while (d > 0)
	{
		for (int i = 0; i < SIZE - d; i++)
		{
			int j = i;
			srav += 1;
			while ((j >= 0) && (mass[j] > mass[j + d]))
			{
				per += 1;
				srav += 1;
				int t = mass[j];
				mass[j] = mass[j + d];
				mass[j + d] = t;
				j--;
			}
		}
		d = d / 2;
	}
	//long double t2 = clock();
	//tt = (t2 - t1) / CLOCKS_PER_SEC;
	QueryPerformanceCounter(&li);
	printf("Stats of the shell sort:\n");
	printf("- Amount of permutation =  %llu\n", per);
	printf("- Amount of comparisons = %llu\n", srav);
	printf("- Time of work = %.15lf seconds\n", (double)(li.QuadPart - CounterStart) / PCFreq);
	//printf("- Time of work = %Lf seconds\n", tt);
	printf("\n");
}
int pq = 0;
int cq = 0;
void quick(int* mass, int first, int last) {
	int i = first, j = last, pivot = mass[(first + last) / 2];
	do {
		cq += 2;
		while (mass[i] < pivot) { i++; cq += 1; }
		while (mass[j] > pivot) { j--; cq += 1; }
		if (i <= j) {
			if (mass[i] > mass[j]) {
				pq += 1;
				cq += 1;
				int t = mass[i];
				mass[i] = mass[j];
				mass[j] = t;
			}	
			i++;
			j--;
		}
	} while (i <= j);

	if (i < last) {
		quick(mass, i, last);
	}
	if (first < j) {
		quick(mass, first, j);
	}
}
int main() {
	printf("Select type:\n1 - file [To work with a text file, first add it to the directory with the solution]\n2 - random\n");
	//Для работы с файлом, надо предварительно добавить его в каталог с решением
	int type = 0;
	int mass[SIZE];
	int mass1[SIZE];
	int mass2[SIZE];
	FILE* f;
	scanf("%d", &type);
	if (type == 1) {
		char filename[256];
		printf("Print filename: ");
		printf("\n");
		scanf("%s", filename);
		f = fopen(filename, "r");
		for (int i = 0; i < SIZE; i++) {
			fscanf(f, "%d", &mass[i]);
		}
		
		fclose(f);
	}
	if (type == 2) {
		srand(time(0));
		for (int i = 0; i < SIZE; i++) {
			mass[i] = rand() % (100 + 100 + 1) - 100;
		}
	}
	for (int i = 0; i < SIZE; i++) {
		mass1[i] = mass[i];
		mass2[i] = mass[i];
	}
	for (int i = 0; i < SIZE; i++) {
		printf("%d ", mass1[i]);
	}
	printf("\n");
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		printf("Error\n");
		return 0;
	}
	PCFreq = (double)(li.QuadPart);
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
				quick(mass2,0,SIZE-1);
	QueryPerformanceCounter(&li);
	printf("Stats of the quick sort:\n");
	printf("- Amount of permutation =  %llu\n", pq);
	printf("- Amount of comparisons = %llu\n", cq);
	printf("- Time of work = %.15lf seconds\n", (double)(li.QuadPart - CounterStart) / PCFreq);
	shell(mass1);
	bubble(mass);
	printf("\n");
	for (int i = 0; i < SIZE; i++) {
		printf("%d ", mass2[i]);
	}
	
}		