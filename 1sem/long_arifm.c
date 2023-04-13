#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int maxlen(char* s1, char* s2) {
	int x = strlen(s1);
	int y = strlen(s2);
	if (x > y) {
		return x;
	}
	else {
		return y;
	}
}

int tr(char x) {
	int y = x - 48;
	if (abs(y) <= 9) {
		return y;
	}
	else {
		return 0;
	}
}

char rt(int x) {
	int y = x + 48;
	char t = y;
	return t;
}

int tm(char* s1, char* s2) {
	int k = 0;
	if (s1[0] == '-') {
		k += 1;
	}
	if (s2[0] == '-') {
		k += 1;
	}
	return k;
}

char* diff(char* s1, char* s2, char m) {
	int len1 = strlen(s1), len2 = strlen(s2), res[1001], x = 0, y = 0, ml = maxlen(s1, s2);
	memset(res, 0, sizeof(int) * 1001);
	int ln1 = 1001;
	int ln2 = 1001;
	char sd[10001];
	int ax[1001];
	int ay[1001];
	memset(ax, 0, sizeof(int) * 1001);
	memset(ay, 0, sizeof(int) * 1001);
	unsigned short int fff = 0;
	unsigned short int signa = 0;
	if ((s1[0] != '-') && (s2[0] != '-')) {
		signa = 2;
	}
	if (s2[0] == '-') {
		signa = 2;
		for (int i = 0; i < len2; i++) {
			s2[i] = s2[i + 1];
		}
		s2[len2 - 1] = '\0';
	}
	if (s1[0] == '-') {
		signa = 1;
		for (int i = 0; i < len1; i++) {
			s1[i] = s1[i + 1];
		}
		s1[len1 - 1] = '\0';
	}
	len1 = strlen(s1);
	len2 = strlen(s2);
	unsigned short int zn = 0;
	if (len1 == len2) {
		for (int j = 0; j < len1; j++) {
			if (s1[j] != s2[j]) {
				fff = 1;
				if (s1[j] > s2[j]) {
					ln1 = len1;
					ln2 = len2;
					for (int i = 0; i < len1; i++) {
						ax[i] = tr(s1[i]);
					}
					for (int i = 0; i < len2; i++) {
						ay[i] = tr(s2[i]);
					}
					if (signa == 1) {
						zn = 1;
					}
					break;
				}
				else {
					ln1 = strlen(s2);
					ln2 = strlen(s1);
					for (int i = 0; i < len2; i++) {
						ax[i] = tr(s2[i]);
					}
					for (int i = 0; i < len1; i++) {
						ay[i] = tr(s1[i]);
					}
					if (signa == 2) {
						zn = 1;
					}
					break;
				}
			}
		}
		char on[] = "0";
		char* onn = on;
		if (fff == 0) {
			return onn;
		}
	}
	if ((len1 > len2)) {
		ln1 = strlen(s1);
		ln2 = strlen(s2);
		if (signa == 1) {
			zn = 1;
		}
		for (int i = 0; i < len1; i++) {
			ax[i] = tr(s1[i]);
		}
		for (int i = 0; i < len2; i++) {
			ay[i] = tr(s2[i]);
		}
	}
	if (len2 > len1) {
		if (signa == 2) {
			zn = 1;
		}
		ln1 = strlen(s2);
		ln2 = strlen(s1);
		for (int i = 0; i < len2; i++) {
			ax[i] = tr(s2[i]);
		}
		for (int i = 0; i < len1; i++) {
			ay[i] = tr(s1[i]);
		}
	}
	int minl = ln2;
	ml = ln1;
	for (int i = 1; i <= ml; i++) {
		x = ax[ln1 - i];
		y = ay[ln2 - i];
		if (ln2 < i) {
			y = 0;
		}
		if (x < y) {
			x += 10;
			ax[ln1 - i - 1] --;
		}
		res[ml - i + 1] = x - y;
		int u = 0;
	}
	int hf = 0;
	if (zn == 1) {
		hf = 1;
		sd[0] = '-';
	}
	unsigned short int flago = 0;
	for (int i = 0; i <= ml; i++) {
		if ((res[i] == 0) && (flago == 0) || ((i == 0) && (zn == 1))) {
			continue;
		}
		else {
			flago = 1;
			sd[hf] = rt(res[i]);
			hf++;
		}
	}
	sd[hf] = '\0';
	return sd;
}

char* sum(char* s1, char* s2, char m) {
	int len1 = strlen(s1), len2 = strlen(s2), res[1001], x = 0, y = 0, ml = maxlen(s1, s2);
	memset(res, 0, sizeof(int) * 1001);
	char sm[10001];
	char gg = s1[0];
	for (int i = 1; i <= ml; i++) {
		x = tr(s1[len1 - i]);
		y = tr(s2[len2 - i]);
		if (x < 0) {
			x = 0;
		}
		if (y < 0) {
			y = 0;
		}
		int z = res[ml - i + 1];
		res[ml - i + 1] = (z + (x + y)) % 10;
		if (((z + (x + y))) >= 10) {
			res[ml - i] = 1;
		}
	}
	int t = tm(s1, s2);
	if (m == '-') {
		t += 1;
	}
	int fl = 0;
	int hg = 0;
	char rrx[1001];
	if (gg != '-') {
		if (res[0] == 0) {
			for (int i = 1; i <= ml; i++) {
				if ((fl == 0) && (res[i] == 0)) {
					continue;
				}
				else {
					fl = 1;
					sm[hg] = rt(res[i]);
					hg += 1;
				}
			}
		}
		else {
			for (int i = 0; i <= ml; i++) {
				sm[hg] = rt(res[i]);
				hg += 1;
			}
		}
	}
	if (gg == '-') {
		sm[0] = '-';
		hg += 1;
		int ggh = 0;
		for (int i = 0; i <= ml; i++) {
			if ((res[i] == 0) && (ggh == 0)) {
				continue;
			}
			else {
				ggh = 1;
				sm[hg] = rt(res[i]);
				hg += 1;
			}
		}
	}
	sm[hg] = '\0';
	return sm;
}

char* multipli(char* s1, char* s2, char m) {
	int len1 = strlen(s1), len2 = strlen(s2), res[10001], res1[10001], x = 0, y = 0, ml = maxlen(s1, s2);
	memset(res, 0, sizeof(int) * 10001);
	memset(res1, 0, sizeof(int) * 10001);
	char sl[10001];
	int ln1 = 1001;
	int ln2 = 1001;
	int ax[10001];
	int ay[10001];
	char on[] = "0";
	char* onn = on;
	if ((s1[0] == '0') || (s2[0] == '0')) {
		return onn;
	}
	memset(ax, 0, sizeof(int) * 10001);
	memset(ay, 0, sizeof(int) * 10001);
	unsigned short int signa = 0;
	if (s2[0] == '-') {
		signa += 1;
		for (int i = 0; i < len2; i++) {
			s2[i] = s2[i + 1];
		}
		s2[len2 - 1] = '\0';
	}
	if (s1[0] == '-') {
		signa += 1;
		for (int i = 0; i < len1; i++) {
			s1[i] = s1[i + 1];
		}
		s1[len1 - 1] = '\0';
	}
	len1 = strlen(s1);
	len2 = strlen(s2);
	unsigned short int zn = 0;
	for (int i = 0; i < len1; i++) {
		ax[i] = tr(s1[i]);
	}
	for (int i = 0; i < len2; i++) {
		ay[i] = tr(s2[i]);
	}
	if ((ax[0] == 0) || (ay[0] == 0)) {
		printf("0");
		return 0;
	}
	int reslen = len1 + len2 + 1;
	for (int i = 0; i < len2; i++) {
		for (int j = 0; j < len1; j++) {
			int tt = (ay[len2 - 1 - i] * ax[len1 - 1 - j]) / 10;
			res[reslen - j] += (ay[len2 - 1 - i] * ax[len1 - 1 - j]) % 10;
			res[reslen - j - 1] += tt;
		}
		for (int h = reslen; h >= 0; h--) {
			int ttt = 0;
			if ((h - i) >= 0) {
				ttt = (res[h] + res1[h - i]) / 10;
				res1[h - i] = (res[h] + res1[h - i]) % 10;
			}
			if ((h - 1 - i) >= 0) {
				res1[h - 1 - i] += ttt;
			}
		}
		memset(res, 0, sizeof(int) * 10001);
	}
	char ss[1001];
	memset(ss, '\0', sizeof(char) * 1001);
	int ffl = 0;
	int u = 0;
	if (signa % 2 == 1) {
		sl[0] = '-';
		u++;
	}
	for (int i = 0; i <= reslen; i++) {
		if ((res1[i] == 0) && (ffl == 0)) {
			continue;
		}
		else {
			ffl = 1;
			sl[u] = rt(res1[i]);
			u++;
		}
	}
	sl[u] = '\0';
	return sl;
}

int degree(char* s1, int deg) {
	if (deg == 0) {
		printf("1");
		return 0;
	}
	int len1 = strlen(s1);
	int proverka = 0;
	unsigned short int signa = 0;
	if (s1[0] == '0' && deg == 0)
	{
		printf("1\n");
		return 0;
	}
	if (s1[0] == '-' && strlen(s1) == 2 && s1[1] == '1')
	{
		proverka = deg % 10;
		if (proverka % 2 == 1)
		{
			printf("-1\n");
			return 0;
		}
		else
		{
			printf("1\n");
			return 0;
		}

	}
	if (s1[0] == '1' && strlen(s1) == 1)
	{
		printf("1");
		return 0;
	}
	if (s1[0] == '-') {
		signa += 1;
		for (int i = 0; i < len1; i++) {
			s1[i] = s1[i + 1];
		}
		s1[len1 - 1] = '\0';
	}
	len1 = strlen(s1);
	char sf[1000];
	char res[10001];
	strcpy(res, s1);
	for (int i = 1; i < deg; i++) {
		strcpy(res, multipli(res, s1, '*'));
	}
	if ((signa == 1) && (deg % 2 == 1)) {
		printf("-");
	}
	printf("%s", res);

}

void fact(char* s1) {
	int len1 = strlen(s1);
	unsigned short int signa = 0;
	int mm = atoi(s1);
	
	if (s1[0] == '-') {
		printf("Error");
		return;
		signa += 1;
		for (int i = 0; i < len1; i++) {
			s1[i] = s1[i + 1];
		}
		s1[len1 - 1] = '\0';
	}
	len1 = strlen(s1);
	char res[10001];
	res[0] = '1';
	res[1] = '\0';
	char* tt;
	char string[10000];
	for (int i = 2; i <= mm; i++) {
		tt = _itoa(i, string, 10);
		strcpy(res, multipli(res, tt, '*'));
	}
	printf("%s", res);
}

void amount(char* s1, char* s2) {
	char rr[1001];
	strcpy(rr, s1);
	char res2[1001];
	strcpy(res2, rr);
	char on[] = "1";
	char* onn = on;
	while ((strcmp(res2, s2)) != 0) {
		if ((rr[0] != '-') && (res2[0] != '-')) {
			strcpy(res2, sum(res2, onn, '+'));
			strcpy(rr, sum(rr, res2, '+'));
		}
		else if ((rr[0] == '-') && (res2[0] == '-')) {
			strcpy(res2, diff(res2, onn, '+'));
			strcpy(rr, sum(rr, res2, '+'));
		}
		else if ((rr[0] == '-') && (res2[0] != '-')) {
			strcpy(res2, sum(res2, onn, '+'));
			strcpy(rr, diff(rr, res2, '+'));
		}
	}
	printf("%s", rr);
}

int main() {
	char s1[1001], s2[1001];
	char opersign;
	memset(s1, '0', sizeof(char) * 1001);
	memset(s2, '0', sizeof(char) * 1001);
	printf("Input number: ");
	scanf("%s", s1);
	printf("\n");
	printf("Input operation sign(+, -, *, ^, !, s): ");
	scanf(" %c", &opersign); //пробел важен
	printf("\n");
	if ((opersign != '!') && (opersign != '^')) {
		printf("Input number: ");
		scanf("%s", s2);
		printf("\n");
		printf("Answer: ");
		int t = tm(s1, s2);
		if (opersign == '-') {
			t += 1;
		}
		if ((opersign == '+') || (opersign == '-')) {
			if ((((t % 2) == 0))) {
				char pp[1001];
				strcpy(pp, sum(s1, s2, opersign));
				fprintf(stdout, "%s", pp);
			}
			else if ((t % 2 == 1) || ((s1[0] == '-') && (s2[0] == '-'))) {
				char pp[1001];
				strcpy(pp, diff(s1, s2, opersign));
				fprintf(stdout, "%s", pp);
			}
		}
		if (opersign == '*') {
			char pp[1001];
			strcpy(pp, multipli(s1, s2, opersign));
			fprintf(stdout, "%s", pp);
		}
		if ((opersign == 's') || (opersign == 'S')) {
			amount(s1, s2);
		}
	}
	else if (opersign == '^') {
		int deg = 0;
		scanf("%d", &deg);
		printf("Answer: ");
		degree(s1, deg);
	}
	else if (opersign == '!') {
		printf("Answer: ");
		fact(s1);
	}
}
