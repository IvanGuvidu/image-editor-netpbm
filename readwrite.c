// Andrei-Cristian IVAN, 311CAa 2024-2025
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readwrite.h"

void parse_word(char s[NMAX], char rez[NMAX], int *poz)
{ // parsam cuvintele din sir
	int n = strlen(s), x = 0;
	while (*poz < n && s[*poz] != ' ' && s[*poz] != '\n') {
		rez[x] = s[*poz]; // cat timp gasim caractere diferite
		(*poz)++; // de spatii si newline, le adaugam in cuvantul
		x++; // extras
	}
	rez[x] = '\0'; // marcam sfarsitul cuvantului
	while (*poz < n && (s[*poz] == ' ' || s[*poz] == '\n'))
		(*poz)++; // trecem peste spatii si newline
}

int parse_nr(char s[NMAX], int *poz)
{ // parsam numerele din sir
	int n = strlen(s), x = 0, sgn = 1;
	if (*poz < n && s[*poz] == '-') {
		sgn = -1; // actualizam semnul
		(*poz)++;
	} // daca deja am ajuns la sfarsitul sirului, sau primul
	if (*poz == n || !(s[*poz] >= '0' && s[*poz] <= '9'))
		return -2; // caracter nu e numar, returam eroare
	while (*poz < n && s[*poz] != ' ' && s[*poz] != '\n') {
		x = x * 10 + (s[*poz] - '0'); // adaugam cifre la numar
		(*poz)++;
	}
	while (*poz < n && (s[*poz] == ' ' || s[*poz] == '\n'))
		(*poz)++; // trecem peste spatii si newline
	return x * sgn;
}

void comments(FILE *in)
{ // dam skip la liniile de comentarii
	fpos_t pos;
	char aux[NMAX];
	while (1) {
		fgetpos(in, &pos); // vom retine pozitia din fisier
		fgets(aux, NMAX, in); // inaintea de citirea liniei
		if (aux[0] != '#') // daca gasim o linie care nu e comentariu ne oprim
			break; // altfel, dam skip
	}
	fsetpos(in, &pos); // setam pozitia in fisier inapoi la
} // cea de la inceputul randului curent

int readtxt(FILE *in)
{ // citirea din fisiere ASCII
	int x;
	fscanf(in, "%d", &x);
	return x;
}

int readbin(FILE *in)
{ // citirea din fisiere binare
	int x;
	unsigned char aux; // tinand cont ca valorile pixelilor incap pe un octet
	// va trebui sa citim cu unsigned char, care are fix un octet
	fread(&aux, sizeof(unsigned char), 1, in);
	x = aux; // transferam valoarea pe int
	return x;
}

void afistxt(int x, FILE *out)
{ // afisam in fisiere ASCII
	fprintf(out, "%d ", x);
}

void afisbin(int x, FILE *out)
{ // afisam in fisiere binare
	unsigned char aux = x;
	// ca la citire, valorile incap pe un octet deci afisam pe
	fwrite(&aux, sizeof(unsigned char), 1, out); // unsigned char
}

void swap(int *x, int *y)
{ // interschimbam doua valori
	int aux = (*x);
	(*x) = (*y);
	(*y) = aux;
}
