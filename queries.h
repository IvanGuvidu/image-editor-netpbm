// Andrei-Cristian IVAN, 311CAa 2024-2025
#pragma once
#include <stdio.h>
#include <math.h>
#include "image.h"
#include "readwrite.h"

void load(imagine * img, selectie * sel, char s[NMAX], int *posrand);
void select(imagine *img, selectie *sel, char s[NMAX], int *posrand);
void rotate(imagine *img, selectie *sel, char s[NMAX], int *posrand);
void equalize(imagine *img);
void crop(imagine *img, selectie *sel);
void apply(imagine *img, selectie sel, char s[NMAX], int *posrand);
void save(imagine *img, char s[NMAX], int *posrand);
void histogram(imagine *img, char s[NMAX], int *posrand);
void exitprogram(imagine *img);
