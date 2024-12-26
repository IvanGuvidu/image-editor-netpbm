// Andrei-Cristian IVAN, 311CAa 2024-2025
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "queries.h"

int main(void)
{
	char s[NMAX];
	imagine img; // vom initializa imaginea
	img.exist = 0;
	selectie sel; // si selectia
	sel.exist = 0;
	while (1) {
		fgets(s, NMAX, stdin); // vom citi rand cu rand
		int posrand = 0; // din stdin si vom
		char aux[NMAX]; // parsa primul cuvant din
		bool amop = 0; // fiecare rand
		parse_word(s, aux, &posrand);
		if (strcmp(aux, "LOAD") == 0) {
			load(&img, &sel, s, &posrand);
			amop = 1; // validam de fiecare data
		} // ca operatia e valida
		if (strcmp(aux, "SELECT") == 0) {
			select(&img, &sel, s, &posrand);
			amop = 1;
		}
		if (strcmp(aux, "CROP") == 0) {
			crop(&img, &sel);
			amop = 1;
		}
		if (strcmp(aux, "SAVE") == 0) {
			save(&img, s, &posrand);
			amop = 1;
		}
		if (strcmp(aux, "APPLY") == 0) {
			apply(&img, sel, s, &posrand);
			amop = 1;
		}
		if (strcmp(aux, "ROTATE") == 0) {
			rotate(&img, &sel, s, &posrand);
			amop = 1;
		}
		if (strcmp(aux, "EQUALIZE") == 0) {
			equalize(&img);
			amop = 1;
		}
		if (strcmp(aux, "HISTOGRAM") == 0) {
			histogram(&img, s, &posrand);
			amop = 1;
		}
		if (strcmp(aux, "EXIT") == 0) {
			exitprogram(&img);
			break;
		} // cazul in care primim o operatie invalida
		if (amop == 0)
			printf("Invalid command\n");
	}
	return 0;
}
