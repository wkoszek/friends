/* Tutaj jest wszystko */
#include <stdio.h>

/* 
 * Tutaj mam zdefiniowane mi�dzy innymi EXIT_FAILURE, EXIT_SUCCESS,
 * czyli kody powrotu z funkcji main()
 */
#include <stdlib.h>

/*
 * Tutaj mam zmienn� wewn�trzn� errno, kt�ra ustawiana jest przez
 * funkcje biblioteki standardowej w przypadku b��du. �eby m�c u�ywa�
 * tych warto�ci i pobra� tekstowy opis b��du, trzeba to doda�.
 */
#include <errno.h>

/*
 * Sta�a oznaczaj�ca nazw� pliku, zdefiniowana jako �atwe do zmiany
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
	 * Nie u�ywam tych dw�ch zmiennych, dlatego rzutuj� je na (void)
	 */
	(void)argc;
	(void)argv;

	/*
	 * Otwieram plik tylko do odczytu.
	 */
	fp = fopen(PLIK, "r");
	if (fp == NULL) {
		/*
		 * Wychodz� z programu z kodem b��du w przypadku problemu
		 */
		fprintf(stderr, "Nie mog� otworzy� pliku %s: %s\n", PLIK, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/*
	 * Sizeof(tab) zwr�ci mi wielko�� tablicy w bajtach.
	 * Sizeof(tab[0]) zwr�ci mi wielko�� pierwszego elementu w
	 * bajtach (czyli ilo�� bajt�w na element tablicy).
	 *
	 * Pierwsze przez drugie da Ci element tablicy.
	 */
	tab_size = sizeof(tab) / sizeof(tab[0]);

	/*
	 * Od pierwszego do ostatniego elementu tablicy...
	 */
	for (i = 0; i < tab_size; i++) {
		/*
		 * Spr�buj wczyta� warto�� "float"...
		 */
		ret = fscanf(fp, "%f", &tab[i]);
		
		/* ...kt�re si� udaje, je�eli ret == 1. W przeciwnym
		 * wypadku wyst�pi� jaki� b��d...
		 */
		if (ret != 1) {
			/*
			 * Kt�ry obs�uguj� poprzez wyj�cie z programu.
			 */
			fprintf(stderr, "Nie mog� przeczyta� %d-tej warto�ci\n", i);
			fclose(fp);
			exit(EXIT_FAILURE);
		}
	}

	/*
	 * Na koniec wypisuj� ca�� tablic�
	 */
	for (i = 0; i < tab_size; i++)
		printf("%f\n", tab[i]);

	/*
	 * Wychodz� z EXIT_SUCCESS, jako i� program ko�czy si�
	 * poprawnie.
	 */
	exit(EXIT_SUCCESS);
}
