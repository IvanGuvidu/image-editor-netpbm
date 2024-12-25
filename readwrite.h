// Andrei-Cristian IVAN, 311CAa 2024-2025
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 1005 // dimensiunea maxima a unei linii din input

void parse_word(char s[NMAX], char rez[NMAX], int *poz);
int parse_nr(char s[NMAX], int *poz);
void comments(FILE *in);
int readtxt(FILE *in);
int readbin(FILE *in);
void afistxt(int x, FILE *out);
void afisbin(int x, FILE *out);
void swap(int *x, int *y);
