/* Tutaj jest wszystko */
#include <stdio.h>

/* 
 * Tutaj mam zdefiniowane miêdzy innymi EXIT_FAILURE, EXIT_SUCCESS,
 * czyli kody powrotu z funkcji main()
 */
#include <stdlib.h>

/*
 * Tutaj mam zmienn± wewnêtrzn± errno, która ustawiana jest przez
 * funkcje biblioteki standardowej w przypadku b³êdu. ¯eby móc u¿ywaæ
 * tych warto¶ci i pobraæ tekstowy opis b³êdu, trzeba to dodaæ.
 */
#include <errno.h>

/*
 * Sta³a oznaczaj±ca nazwê pliku, zdefiniowana jako ³atwe do zmiany
 * makro.
 */
#define	PLIK	"dane.txt"

/*
 * Ile liczb wczytujemy
 */
#define TABSIZE	20

int
main(int argc, char **argv)
{
	FILE *fp;
	float tab[TABSIZE];
	int tab_size;
	int ret;
	int i;

	/*
	 * Nie u¿ywam tych dwóch zmiennych, dlatego rzutujê je na (void)
	 */
	(void)argc;
	(void)argv;

	/*
	 * Otwieram plik tylko do odczytu.
	 */
	fp = fopen(PLIK, "r");
	if (fp == NULL) {
		/*
		 * Wychodzê z programu z kodem b³êdu w przypadku problemu
		 */
		fprintf(stderr, "Nie mogê otworzyæ pliku %s: %s\n", PLIK, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/*
	 * Sizeof(tab) zwróci mi wielko¶æ tablicy w bajtach.
	 * Sizeof(tab[0]) zwróci mi wielko¶æ pierwszego elementu w
	 * bajtach (czyli ilo¶æ bajtów na element tablicy).
	 *
	 * Pierwsze przez drugie da Ci element tablicy.
	 */
	tab_size = sizeof(tab) / sizeof(tab[0]);

	/*
	 * Od pierwszego do ostatniego elementu tablicy...
	 */
	for (i = 0; i < tab_size; i++) {
		/*
		 * Spróbuj wczytaæ warto¶æ "float"...
		 */
		ret = fscanf(fp, "%f", &tab[i]);
		
		/* ...które siê udaje, je¿eli ret == 1. W przeciwnym
		 * wypadku wyst±pi³ jaki¶ b³±d...
		 */
		if (ret != 1) {
			/*
			 * Który obs³ugujê poprzez wyj¶cie z programu.
			 */
			fprintf(stderr, "Nie mogê przeczytaæ %d-tej warto¶ci\n", i);
			fclose(fp);
			exit(EXIT_FAILURE);
		}
	}

	/*
	 * Na koniec wypisujê ca³± tablicê
	 */
	for (i = 0; i < tab_size; i++)
		printf("%f\n", tab[i]);

	/*
	 * Wychodzê z EXIT_SUCCESS, jako i¿ program koñczy siê
	 * poprawnie.
	 */
	exit(EXIT_SUCCESS);
}
