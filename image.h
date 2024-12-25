// Andrei-Cristian IVAN, 311CAa 2024-2025
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "readwrite.h"

#define CMAX 3 // numarul de canale maxime

typedef struct pixel {
	int c[CMAX];
} pixel;

typedef struct imagine {
	bool exist;
	int n, m, type, maxval;
	pixel **v;
} imagine;

typedef struct submatrix {
	bool exist;
	int x1, y1, x2, y2;
} selectie;

void del_img(imagine *img);
void read_img(imagine *img, selectie *sel, FILE *in);
void afis_img(imagine img, FILE *out);
void init_ker(char s[NMAX], int ker[CMAX][CMAX], double *imp);
void clamp(int *x, int minim, int maxim);
bool checksq(selectie sel);
bool checkfull(selectie sel, imagine img);
bool checkval(int x, int y);
void rot90(imagine *img, selectie *sel);
void rot180(imagine *img, selectie *sel);
void rot270(imagine *img, selectie *sel);
