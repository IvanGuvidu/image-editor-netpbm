// Andrei-Cristian IVAN, 311CAa 2024-2025
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "readwrite.h"

void del_img(imagine *img)
{ // stergem imaginea
	for (int i = 0; i < img->n; i++) {
		free(img->v[i]);
	}
	free(img->v);
	img->exist = 0;
}

void read_img(imagine *img, selectie *sel, FILE  *in)
{ // citim imaginea
	img->exist = 1;
	img->v = malloc(img->n * sizeof(pixel *));
	for (int i = 0; i < img->n; i++)
		img->v[i] = malloc(img->m * sizeof(pixel));
	if (!img) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	for (int i = 0; i < img->n; i++)
		for (int j = 0; j < img->m; j++) {
			if (img->type == 2) // in functie de tipul imaginii
				img->v[i][j].c[0] = readtxt(in);
			if (img->type == 5) // vom avea citiri diferite
				img->v[i][j].c[0] = readbin(in);
			if (img->type == 3)
				for (int k = 0; k < 3; k++)
					img->v[i][j].c[k] = readtxt(in);
			if (img->type == 6)
				for (int k = 0; k < 3; k++)
					img->v[i][j].c[k] = readbin(in);
		}
	sel->exist = 1;
	sel->y1 = 0; // initializam selectia sa cuprinda
	sel->x1 = 0; // initial toata imaginea
	sel->y2 = img->m;
	sel->x2 = img->n;
}

void afis_img(imagine img, FILE *out)
{ // afisam imaginea
	for (int i = 0; i < img.n; i++) {
		for (int j = 0; j < img.m; j++) {
			if (img.type == 2) // in functie de tipul imaginii
				afistxt(img.v[i][j].c[0], out);
			if (img.type == 3) // vom avea afisari diferite
				for (int k = 0; k < 3; k++)
					afistxt(img.v[i][j].c[k], out);
			if (img.type == 5)
				afisbin(img.v[i][j].c[0], out);
			if (img.type == 6)
				for (int k = 0; k < 3; k++)
					afisbin(img.v[i][j].c[k], out);
		}
		if (img.type <= 3)
			fprintf(out, "\n");
	}
}

void init_ker(char s[NMAX], int ker[CMAX][CMAX], double *imp)
{ // initializam nucleul
	if (strcmp(s, "SHARPEN") == 0) {
		(*imp) = 1;
		ker[0][0] = 0;
		ker[0][2] = 0;
		ker[2][0] = 0;
		ker[2][2] = 0;
		ker[0][1] = -1;
		ker[1][0] = -1;
		ker[2][1] = -1;
		ker[1][2] = -1;
		ker[1][1] = 5;
	}
	if (strcmp(s, "EDGE") == 0) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				ker[i][j] = -1;
		ker[1][1] = 8;
		(*imp) = 1;
	}
	if (strcmp(s, "BLUR") == 0) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				ker[i][j] = 1;
		(*imp) = 9;
	}
	if (strcmp(s, "GAUSSIAN_BLUR") == 0) {
		(*imp) = 16;
		ker[0][0] = 1;
		ker[0][2] = 1;
		ker[2][0] = 1;
		ker[2][2] = 1;
		ker[0][1] = 2;
		ker[1][0] = 2;
		ker[2][1] = 2;
		ker[1][2] = 2;
		ker[1][1] = 4;
	}
}

void clamp(int *x, int minim, int maxim)
{ // functia de clamp, ne asiguram ca valorile
	if ((*x) < minim) // pixelilor sunt in intervalul
		(*x) = minim; // dorit
	if ((*x) > maxim)
		(*x) = maxim;
}

bool checksq(selectie sel)
{ // verificam daca selectia este un patrat
	int dx = sel.x2 - sel.x1;
	int dy = sel.y2 - sel.y1;
	if (dx != dy)
		return 0;
	return 1;
}

bool checkfull(selectie sel, imagine img)
{ // verificam daca selectia cuprinde toata imaginea
	if (sel.x1 != 0 || sel.y1 != 0)
		return 0;
	if (sel.x2 != img.n || sel.y2 != img.m)
		return 0;
	return 1;
}

bool checkval(int x, int y)
{ // verificam daca valoarea data se afla in intervalul dorit
	if (x >= 0 && x <= y)
		return 1;
	return 0;
}

void rot90(imagine *img, selectie *sel)
{ // rotim la 90 de grade imaginea
	imagine aux; // vom initializa o copie inainte
	aux.maxval = img->maxval;
	aux.type = img->maxval; // transferam caracteristicile
	aux.exist = 1; // imaginii
	aux.type = img->type;
	if (checkfull(*sel, *img) == 1) { // daca rotim complet imaginea
		aux.n = img->m; // dimensiunile le inversam
		aux.m = img->n;
	} else { // altfel isi pastreaza dimensiunile
		aux.n = img->n;
		aux.m = img->m;
	}
	aux.v = malloc(aux.n * sizeof(pixel *));
	for (int i = 0; i < aux.n; i++)
		aux.v[i] = malloc(aux.m * sizeof(pixel));
	if (!aux.v) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	if (checkfull(*sel, *img) == 1) { // caz particular pentru
		for (int i = 0; i < img->n; i++) // intreaga imagine rotita
			for (int j = 0; j < img->m; j++) { // scoatem coordonatele noi
				int newi = j, newj = img->n - i - 1; // in imaginea rotita
				for (int k = 0; k < 3; k++)
					aux.v[newi][newj].c[k] = img->v[i][j].c[k];
			}
		del_img(img);
		*img = aux;
		swap(&sel->x2, &sel->y2); // si selectia se inverseaza
		return;
	}
	for (int i = 0; i < aux.n; i++)
		for (int j = 0; j < aux.m; j++)
			for (int k = 0; k < 3; k++) // copiem pixelii originali
				aux.v[i][j].c[k] = img->v[i][j].c[k]; // ca o sa fie o parte din
	for (int i = sel->x1; i < sel->x2; i++) // imagine care nu este rotita
		for (int j = sel->y1; j < sel->y2; j++) {
			int newi = j - sel->y1 + sel->x1; // scoatem coordonatele noi
			int newj = sel->y2 - (i - sel->x1) - 1;
			for (int k = 0; k < 3; k++)
				aux.v[newi][newj].c[k] = img->v[i][j].c[k];
		}
	del_img(img);
	*img = aux;
}

void rot180(imagine *img, selectie *sel)
{ // rotim la 180 de grade imaginea
	imagine aux; // initializam o copie
	aux.maxval = img->maxval;
	aux.type = img->maxval; // fata de cazul de 90 de grade,
	aux.exist = 1; // la 180, indiferent daca rotim sau nu
	aux.n = img->n; // imaginea complet, dimensiunile se
	aux.m = img->m; // pastreaza
	aux.type = img->type;
	aux.v = malloc(aux.n * sizeof(pixel *));
	for (int i = 0; i < aux.n; i++)
		aux.v[i] = malloc(aux.m * sizeof(pixel));
	if (!aux.v) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	for (int i = 0; i < aux.n; i++)
		for (int j = 0; j < aux.m; j++) // copiem posibili pixeli
			for (int k = 0; k < 3; k++) // nemodificati
				aux.v[i][j].c[k] = img->v[i][j].c[k];
	for (int i = sel->x1; i < sel->x2; i++)
		for (int j = sel->y1; j < sel->y2; j++) {
			int newi = sel->x1 + sel->x2 - i - 1; // scoatem coordonatele noi
			int newj = sel->y1 + sel->y2 - j - 1;
			for (int k = 0; k < 3; k++)
				aux.v[newi][newj].c[k] = img->v[i][j].c[k];
		}
	del_img(img);
	*img = aux;
}

void rot270(imagine *img, selectie *sel)
{ // rotim la 270 de grade imaginea
	imagine aux; // initializam o copie
	aux.maxval = img->maxval;
	aux.type = img->maxval;
	aux.exist = 1;
	aux.type = img->type; // ca in cazul de 90 de grade, daca
	if (checkfull(*sel, *img) == 1) { // rotim complet imaginea,
		aux.n = img->m; // schimbam si dimensiunile intre ele
		aux.m = img->n;
	} else {
		aux.n = img->n;
		aux.m = img->m;
	}
	aux.v = malloc(aux.n * sizeof(pixel *));
	for (int i = 0; i < aux.n; i++)
		aux.v[i] = malloc(aux.m * sizeof(pixel));
	if (!aux.v) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	if (checkfull(*sel, *img) == 1) { // caz particular pentru rotirea intregii
		for (int i = 0; i < img->n; i++) // imagini
			for (int j = 0; j < img->m; j++) { // scoatem coordonatele noi
				int newi = sel->y2 - j - 1, newj = i; // in imaginea rotita
				for (int k = 0; k < 3; k++)
					aux.v[newi][newj].c[k] = img->v[i][j].c[k];
			}
		del_img(img);
		*img = aux; // ca la 90 de grade, si selectia se inverseaza
		swap(&sel->x2, &sel->y2);
		return;
	}
	for (int i = 0; i < aux.n; i++)
		for (int j = 0; j < aux.m; j++)
			for (int k = 0; k < 3; k++) // copiem posibili pixeli nemodificati
				aux.v[i][j].c[k] = img->v[i][j].c[k];
	for (int i = sel->x1; i < sel->x2; i++)
		for (int j = sel->y1; j < sel->y2; j++) {
			int newi = sel->y2 - j - 1 + sel->x1; // scoatem coordonatele noi
			int newj = sel->y1 + i - sel->x1; // in imaginea rotita
			for (int k = 0; k < 3; k++)
				aux.v[newi][newj].c[k] = img->v[i][j].c[k];
		}
	del_img(img);
	*img = aux;
}
