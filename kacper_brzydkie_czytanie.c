#include <stdio.h>
#include <stdlib.h>

#define TABSIZE 20

int
main()
{
	FILE *fp;
	float tab[TABSIZE];
	int i;


	fp = fopen("dane.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Nie mogê otworzyæ pliku dane.txt");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < TABSIZE; i++) {
		fscanf(fp, "%f", &tab[i]);
	}
	for (i = 0; i < TABSIZE; i++)
		printf("%f\n", tab[i]);
	exit(EXIT_SUCCESS);
}
