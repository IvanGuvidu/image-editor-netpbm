// Andrei-Cristian IVAN, 311CAa 2024-2025
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "queries.h"
#include "image.h"

void load(imagine *img, selectie *sel, char s[NMAX], int *posrand)
{ // LOAD
	char filename[NMAX];
	int idx = 0;
	if (img->exist == 1) { // daca exista o imagine precedenta
		del_img(img); // o stergem
		sel->exist = 0; // resetam si selectia
	}
	parse_word(s, filename, posrand); // extragem calea fisierului
	FILE *in = fopen(filename, "r"); // si il deschidem
	if (!in) { // verificam ca am reusit sa deschidem acel fisier
		printf("Failed to load %s\n", filename);
		return;
	}
	char aux[CMAX], rezerva[NMAX]; // cand deschizi din output fisiere
	parse_word(filename, rezerva, &idx); // risti sa pierzi calea originala
	fscanf(in, "%s\n", aux); // citim tipul imaginii
	comments(in); // dam skip la liniile de comentarii
	img->type = aux[1] - '0';
	comments(in);
	fscanf(in, "%d %d\n", &img->m, &img->n); // extragem dimensiunile
	comments(in);
	fscanf(in, "%d\n", &img->maxval); // extragem valoarea maxima
	comments(in); // a pixelilor
	read_img(img, sel, in); // citim continutul imaginii
	printf("Loaded %s\n", rezerva); // confirmam incarcarea
	fclose(in); // si inchidem fisierul imaginii
}

void select(imagine *img, selectie *sel, char s[NMAX], int *posrand)
{ // SELECT
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	char selection[NMAX];
	int posvechi = *posrand, x1, y1, x2, y2;
	parse_word(s, selection, posrand); // verificam daca e SELECT ALL
	if (strcmp(selection, "ALL") == 0) {
		sel->x1 = 0;
		sel->y1 = 0;
		sel->x2 = img->n;
		sel->y2 = img->m;
		sel->exist = 1;
		printf("Selected ALL\n");
		return;
	}
	*posrand = posvechi; // apoi vom incerca sa parsam numerele
	y1 = parse_nr(s, posrand);
	x1 = parse_nr(s, posrand);
	y2 = parse_nr(s, posrand);
	x2 = parse_nr(s, posrand);
	int n = strlen(s);
	if (*posrand != n) { // verificam daca am parsat complet selectia
		printf("Invalid command\n");
		return;
	}
	if (x1 == -2 || y1 == -2 || x2 == -2 || y2 == -2) {
		printf("Invalid command\n"); // si ca numerele sunt valide
		return;
	}
	if (!checkval(x1, img->n) || !checkval(y1, img->m)) {
		printf("Invalid set of coordinates\n");
		return; // verificam daca coordonatele sunt in imagine
	}
	if (!checkval(x2, img->n) || !checkval(y2, img->m)) {
		printf("Invalid set of coordinates\n");
		return;
	} // selectiile sunt de tipul [A, B), deci daca am selectie
	if (x1 == x2 || y1 == y2) { // de forma [A, A)
		printf("Invalid set of coordinates\n");
		return; // inseamna ca nu am selectat nimic
	}
	sel->exist = 1; // actualizez selectia curenta
	sel->x1 = x1;
	sel->y1 = y1;
	sel->x2 = x2;
	sel->y2 = y2;
	if (sel->x1 > sel->x2) {
		swap(&sel->x1, &sel->x2);
	} // ma asigur ca x1 < x2 si y1 < y2
	if (sel->y1 > sel->y2) {
		swap(&sel->y1, &sel->y2);
	} // si confirmam selectia
	printf("Selected %d %d %d %d\n", sel->y1, sel->x1, sel->y2, sel->x2);
}

void crop(imagine *img, selectie *sel)
{ // CROP
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	imagine aux; // initializam o copie
	aux.m = sel->y2 - sel->y1; // dimensiunile noi ale imaginii
	aux.n = sel->x2 - sel->x1;
	aux.type = img->type; // copiem pixelu maxim
	aux.maxval = img->maxval; // si tipul imaginii
	aux.exist = 1;
	aux.v = malloc(aux.n * sizeof(pixel *));
	for (int i = 0; i < aux.n; i++) // ne pregatim sa copiem pixelii
		aux.v[i] = malloc(aux.m * sizeof(pixel));
	if (!aux.v) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	for (int i = 0; i < aux.n; i++)
		for (int j = 0; j < aux.m; j++)
			aux.v[i][j] = img->v[i + sel->x1][j + sel->y1];
	del_img(img); // stergem imaginea veche
	*img = aux; // pentru a o copia pe cea noua
	sel->y1 = 0;
	sel->x1 = 0; // actualizam selectia
	sel->y2 = img->m; // si confirmam croparea imaginii
	sel->x2 = img->n;
	printf("Image cropped\n");
}

void save(imagine *img, char s[NMAX], int *posrand)
{ // SAVE
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	char filename[NMAX], opt2[NMAX];
	int txt = 0; // o sa extragem calea fisierului nou
	// si daca se vrea sa o afisam ASCII sau binar
	parse_word(s, filename, posrand);
	parse_word(s, opt2, posrand);
	FILE *out = fopen(filename, "w");
	if (strcmp(opt2, "ascii") == 0)
		txt = 1;
	if (txt == 1 && img->type > 3)
		img->type -= 3; // actualizam tipul imaginii
	if (txt == 0 && img->type <= 3)
		img->type += 3; // conform dorintelor de afisare
	fprintf(out, "P%d\n%d %d\n%d\n", img->type, img->m, img->n, img->maxval);
	afis_img(*img, out); // afisam imaginea
	printf("Saved %s\n", filename);
	fclose(out); // confirmam afisarea ei si inchidem fisierul
}

void apply(imagine *img, selectie sel, char s[NMAX], int *posrand)
{ // APPLY
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	char aux[NMAX]; // vom parsa filtrul dorit
	int opvalid = 0, n = strlen(s);
	parse_word(s, aux, posrand);
	if (aux[0] == '\0') {
		printf("Invalid command\n");
		return;
	} // ne asiguram ca extragem un filtru valid
	if (n != (*posrand)) {
		printf("APPLY parameter invalid\n");
		return;
	}
	if (strcmp(aux, "SHARPEN") == 0 || strcmp(aux, "BLUR") == 0)
		opvalid = 1;
	if (strcmp(aux, "GAUSSIAN_BLUR") == 0 || strcmp(aux, "EDGE") == 0)
		opvalid = 1;
	if (opvalid == 0) {
		printf("APPLY parameter invalid\n");
		return;
	}
	if (img->type == 2 || img->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return; // nu putem aplica filtre pe imagini grayscale
	}
	imagine newimg; // initializam o copie
	int ker[CMAX][CMAX]; // in ker vom tine filtrul
	double imp = 1; // cu cat vom imparti la final
	// linear algebra trauma
	newimg.n = img->n; // caracteristicile pozei raman
	newimg.m = img->m; // aceleasi, mai putin pixelii
	newimg.type = img->type;
	newimg.maxval = img->maxval;
	newimg.exist = 1;
	newimg.v = malloc(newimg.n * sizeof(pixel *));
	for (int i = 0; i < newimg.n; i++)
		newimg.v[i] = malloc(newimg.m * sizeof(pixel));
	if (!newimg.v) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	for (int i = 0; i < newimg.n; i++)
		for (int j = 0; j < newimg.m; j++)
			for (int k = 0; k < 3; k++) // copiem pixelii vechi
				newimg.v[i][j].c[k] = img->v[i][j].c[k];
	// pentru ca noi vom aplica nucleul pe selectie, nu pe toata poza
	init_ker(aux, ker, &imp);
	if (sel.x1 == 0) // ne asiguram ca vom lua mereu numai celule
		sel.x1++; // care au mereu toti vecinii in matrice
	if (sel.y1 == 0) // deci celulele de pe chenar nu sunt bune
		sel.y1++;
	if (sel.x2 == newimg.n)
		sel.x2--;
	if (sel.y2 == newimg.m)
		sel.y2--;
	for (int i = sel.x1; i < sel.x2; i++)
		for (int j = sel.y1; j < sel.y2; j++)
			for (int k = 0; k < 3; k++) {
				double sum = 0;
				int ix = i - 1, jx = j - 1;
				// scoatem suma
				for (int ii = 0; ii < 3; ii++)
					for (int jj = 0; jj < 3; jj++) {
						int x = img->v[ix + ii][jx + jj].c[k];
						x *= ker[ii][jj];
						sum += x;
					} // pe care o impartim
				newimg.v[i][j].c[k] = sum / imp;
				clamp(&newimg.v[i][j].c[k], 0, 255);
			} // si cu clamp ne validam pixelii
	del_img(img); // stergem imaginea veche
	*img = newimg; // o copiem pe cea noua
	printf("APPLY %s done\n", aux); // si confirmam
} // aplicarea nucleului

void rotate(imagine *img, selectie *sel, char s[NMAX], int *posrand)
{ // ROTATE
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	int x, n = strlen(s); // vom parsa unghiul
	x = parse_nr(s, posrand);
	if (*posrand != n || x == -2) {
		printf("Invalid command\n");
		return;
	} // verificam ca am extras un unghi valid
	if (x % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	} // verificam daca selectia este valabila pentru rotatie
	if (!checksq(*sel) && !checkfull(*sel, *img)) {
		printf("The selection must be square\n");
		return; // fie e toata imaginea, fie selectia e patrat
	}
	int rot = x / 90; // scoatem numarul de rotatii, pe care-l vrem
	if (rot < 0) // modulo 4
		rot = 4 - (abs(rot) % 4);
	else // ca rotatiile sunt modulo 360 congruente
		rot %= 4;
	if (rot == 1) // in functie de restul la 4, rotim cu unghiul
		rot90(img, sel); // selectat
	if (rot == 2) // o rotatie de 360 de grade este echivalent
		rot180(img, sel); // cu o rotatie de 0 grade, adica nimic
	if (rot == 3)
		rot270(img, sel);
	printf("Rotated %d\n", x); // confirmam rotirea imaginii
}

void histogram(imagine *img, char s[NMAX], int *posrand)
{ // HISTOGRAM
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	int x, y, n = strlen(s); // vom parsa parametri
	x = parse_nr(s, posrand);
	y = parse_nr(s, posrand);
	if (y == -2 || x == -2 || *posrand != n) {
		printf("Invalid command\n");
		return;
	} // verificam ca parametri sunt valizi
	if ((y & (y - 1)) != 0 || y == 0) {
		printf("Invalid command\n");
		return;
	} // histograma se aplica doar pe imagini grayscale
	if (img->type == 3 || img->type == 6) {
		printf("Black and white image needed\n");
		return;
	} // ne vom initializa un vector de frecventa
	int *fr = malloc(y * sizeof(int)), range = (img->maxval + 1) / y;
	if (!fr) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	for (int i = 0; i < y; i++) // si vom afla cate numere incap
		fr[i] = 0; // intr-un bin
	for (int i = 0; i < img->n; i++)
		for (int j = 0; j < img->m; j++)
			fr[img->v[i][j].c[0] / range]++; // actualizam frecventele
	int mx = 0;
	for (int i = 0; i < y; i++)
		if (fr[i] > mx) // aflam frecventa maxima
			mx = fr[i];
	for (int i = 0; i < y; i++) {
		int stele = x * fr[i] / mx;
		printf("%d\t|\t", stele); // scoatem numarul de stele
		for (int i = 1; i <= stele; i++)
			printf("*"); // si afisam
		printf("\n");
	}
	free(fr); // eliberam la final vectorul de frecventa
}

void equalize(imagine *img)
{ // EQUALIZE
	if (img->exist == 0) { // ne asiguram ca exista imagine
		printf("No image loaded\n");
		return;
	}
	if (img->type == 3 || img->type == 6) {
		printf("Black and white image needed\n");
		return; // se aplica doar pe imagini grayscale
	}
	int *fr = malloc((img->maxval + 1) * sizeof(int)), arie = img->n * img->m;
	if (!fr) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	// ne vom initializa vectorul de frecventa
	for (int i = 0; i <= img->maxval; i++)
		fr[i] = 0;
	for (int i = 0; i < img->n; i++)
		for (int j = 0; j < img->m; j++)
			fr[img->v[i][j].c[0]]++; // facem frecventele
	int *h = malloc((img->maxval + 1) * sizeof(int));
	if (!h) {
		printf("Memory allocation failed\n");
		del_img(img);
		exit(0);
	}
	// pentru a nu calcula de fiecare data acea suma, vom face un tablou
	h[0] = fr[0]; // de sume partiale pe frecvente
	for (int i = 1; i <= img->maxval; i++)
		h[i] = h[i - 1] + fr[i];
	for (int i = 0; i < img->n; i++)
		for (int j = 0; j < img->m; j++) {
			int x = img->v[i][j].c[0]; // aplicam formula pentru pixelul nou
			img->v[i][j].c[0] = round((double)255 / arie * h[x]);
			clamp(&img->v[i][j].c[0], 0, 255); // validam pixelu
		}
	printf("Equalize done\n"); // confirmam aplicarea operatiei
	free(fr); // si eliberam vectorii
	free(h);
}

void exitprogram(imagine *img)
{ // EXIT
	if (img->exist == 0) {
		printf("No image loaded\n");
		return; // nu putem sterge ceva ce nu exista
	}
	del_img(img); // stergem imaginea
}
