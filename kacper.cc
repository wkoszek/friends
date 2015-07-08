#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <ncurses.h>

using namespace std;

#define UP 'u'
#define	DOWN 'j'
#define LEFT 'h'
#define RIGHT 'k'

/*
 * Nasz robot.
 */
class Robot {
	private:
		int znak; /* jak na planszy reprezentujemy robota */
		int xpos; /* pocz¹tek planszy po Xach */
		int ypos; /* - || -              Yach */
		int xmax; /* maksymalne X robota */
		int ymax; /* maksymalne Y robota */
		int oldx; /* poprzednie polozenie robota X */
		int oldy; /* -||- dla Y */

	public:
		/*
		 * Konstruktor domyslny
		 */
		Robot()
		{

			znak = '$';
			xpos = 0;
			ypos = 0;
			xmax = 0;
			ymax = 0;
			oldx = 0;
			oldy = 0;
		}

		/*
		 * Tworzymy robota.
		 */
		Robot(int znaczek, int startx, int starty, int maxx, int maxy)
		{

			znak = znaczek;
			xpos = startx;
			ypos = starty;
			xmax = maxx;
			ymax = maxy;
			oldx = xpos;
			oldy = ypos;
		}

		/*
		 * Zwracamy obecne X
		 */
		int&
		X(void)
		{
		
			return (xpos);
		}

		/* Zwracamy obecne Y */
		int&
		Y(void)
		{
		
			return (ypos);
		}

		/* Zwracamy stare X */
		int&
		oldX(void)
		{

			return (oldx);
		}

		/* Zwracamy stare Y */
		int&
		oldY(void)
		{

			return (oldy);
		}

		/* Zwracamy znak robota */
		int&
		Znak(void)
		{

			return (znak);
		}

		/* Wykonujemy ruch robota i zmieniamy wspolrzedne */
		int
		Rusz(int key)
		{
			/* Uwaga. Zmienne zaraz ulegna zmianie.  Obecne
			 * ustawienia staja sie starymi.
			 */
			oldx = xpos;
			oldy = ypos;

			/* 
			 * W zaleznosci od klawicza zmien pozycje
			 */
			if (key == LEFT) xpos--;
			if (key == RIGHT) xpos++;
			if (key == DOWN) ypos++;
			if (key == UP) ypos--;

			/* 
			 * Zadna z pozycji nie moze wybiegac poza minimum
			 * polozenia (0) oraz jego maksymalna wartosc
			 */
			if (xpos < 0) xpos = 0;
			if (xpos >= xmax) xpos = xmax - 1;

			if (ypos < 0) ypos = 0;
			if (ypos >= ymax) ypos = ymax - 1;

			return 0;
		}
};

/*
 * Plansza, po której robot mo¿e siê poruszaæ.
 */
class PlanszaXY {
	private:
		/*
		 * Gdzie trzymamy plansze z literkami
		 */
		char **plansza_xy;

		/*
		 * Wielkosc naszej planszy.
		 */
		int x;
		int y;

		/*
		 * Nasz robot.
		 */
		Robot *robo;

		/*
		 * Opis przeszkody -- StartX/Y, EndX/Y
		 */
		struct {
			int sx;
			int sy;
			int ex;
			int ey;
		} przeszkoda;

		/*
		 * Gdzie w menu jestesmy.
		 */
		enum {
			MENU_GLOWNE,
			MENU_PORUSZANIA,
			MENU_INTELI
		} gdzie;

	public:

		/*
		 * Konstruktor domy¶lny.
		 */
		PlanszaXY()
		{

			/* Dla pewno¶ci */
			plansza_xy = NULL;
			x = -1;
			y = -1;
			robo = NULL;
			przeszkoda.sx = 0;
			przeszkoda.sy = 0;
			przeszkoda.ex = 0;
			przeszkoda.ey = 0;
		}

		/*
		 * Czysci plansze wpisujac znak "wypelnienia
		 */
		void
		PlanszaXY_Czysc(void)
		{
			int xi, yi;

			for (xi = 0; xi < x; xi++) {
				for (yi = 0; yi < y; yi++) {
					plansza_xy[xi][yi] = '.';
				}
			}
		}

		/*
 		 * Konstruuje plansze zgodnie z przekazanymi
		 * wartoœciami.
		 *
		 * robo musi byc obiektem klasy Robot! (wczesniej musi zostac
		 * stworzony).
		 */
		PlanszaXY(int _x, int _y, Robot *rob)
		{
			int xi;

			assert(_x > 0);
			x = _x;
			assert(_y > 0);
			y = _y;
			assert(rob != NULL);
			robo = rob;

			plansza_xy = new char* [_x];
			assert(plansza_xy != NULL);
			for (xi = 0; xi < _x; xi++)
				plansza_xy[xi] = new char[_y];
			PlanszaXY_Czysc();
		}

		/*
		 * Rysujemy plansze 
		 */
		void
		Narysuj(void)
		{
			int xi, yi;
			int ile_zajmuje_stary_ekran;

			/*
			 * O ile wypadaloby przesunac ekran w dol (ile znakow
			 * nowej linii wpisac), zebysmy cos juz powoli
			 * zobaczyli, bez sugerowania sie tym, co jest
			 * posrodku ekranu.
			 *
			 * Wartosc 14 jest dobrana eksperymentalnie i dziala
			 * dobrze w moim terminalu.
			 */
			ile_zajmuje_stary_ekran = y + 14;
			for (yi = 0; yi < ile_zajmuje_stary_ekran; yi++)
				cout << endl;

			/*
			 * Dla kazdego pola planszy...
			 */
			for (yi = 0; yi < y; yi++) {
				for (xi = 0; xi < x; xi++) {
					/* Zobacz, czy to pole jest miejscem,
					 * w ktorym stoi nasz robot.
					 */
					if (xi == robo->X() && yi == robo->Y()) {
						/* 
						 * Jezeli tak, to zamiast pola
						 * "wypelnienia" wstaw znak
						 * naszego robota
						 */
						cout << (char)robo->Znak();
					} else {
						/*
						 * Jezeli nie, to wypisz
						 * wypelnienie.
						 */
						cout << plansza_xy[xi][yi];
					}
				}
				cout << endl;
			}
		}

		/*
		 * Co mozemy robic tuz po uruchomieniu programu oraz w jego
		 * "oknie glownym".
		 */
		void
		Menu_Glowne(void)
		{

			gdzie = MENU_GLOWNE;
	
			cout << "--- Instrukcja obslugi programu ---" << endl;
			cout << "'1' - tryb wprowadzania danych" << endl;
			cout << "'2' - tryb poruszania sie" << endl;
			cout << "'3' - tryb inteligentny (jeszcze nie jest dopisany)"
				<< endl;
			cout << "'4' - koniec programu" << endl;
		}

		/*
		 * Wprowadzamy opis przeszkody
		 */
		void
		Menu_Wprowadzania(void)
		{
			int sx, sy, ex, ey;

			cout << "Podaj parametry przeszkody:" << endl;
			cout << "X poczatkowe: " << endl; cin >> sx;
			cout << "Y poczatkowe: " << endl; cin >> sy;
			cout << "X koncowe: " << endl; cin >> ex;
			cout << "Y koncowe: " << endl; cin >> ey;
			Przeszkoda_Ustaw(sx, sy, ex, ey);
			cout << "----------------------" << endl;
			cout << "Przeszkoda wprowadzona" << endl;
			cout << "----------------------" << endl;
			Menu_Glowne();
		}

		/*
		 * Jak poruszac sie po naszej planszy
		 */
		void
		Menu_Poruszania_Sie(void)
		{
			gdzie = MENU_PORUSZANIA;
		
			cout << "------------------- Jak sterowac ----------" << endl;
			cout << UP << "- do gory" << endl;
			cout << DOWN << "- w dol" << endl;
			cout << LEFT << "- lewo" << endl;
			cout << RIGHT << "- prawo" << endl;
			cout << "----" << endl;
			cout << "g - powrot do glownego menu" << endl;
			cout << "'4' - koniec programu" << endl;
		}

		/*
		 * Do zaimplementowania
		 */
		void
		Menu_Tryb_I(void)
		{
		
			gdzie = MENU_INTELI;
			cout << "Jeszcze nie ma" << endl;
		}

		/*
		 * Ustawienie przeszkody na planszy
		 */
		int
		Przeszkoda_Ustaw(int _sx, int _sy, int _ex, int _ey)
		{

			/* 
			 * Najpierw troche testow zeby nie okazalo sie
			 * ze chcemy ustawic przeszkode poza plansza
			 */
			if (_sx < 0 || _sx >= x) {
				cerr << "Zly X (spoza zakresu 0 i " << x <<
					")";
				return (-1);
			}
			if (_sy < 0 || _sy >= y) {
				cerr << "Zly Y (spoza zakresu 0 i " << y <<
					")";
				return (-1);
			}
			if (_ex < 0 || _ex >= x) {
				cerr << "Zly X (spoza zakresu 0 i " << x <<
					")";
				return (-1);
			}
			if (_ey < 0 || _ey >= y) {
				cerr << "Zly Y (spoza zakresu 0 i " << y <<
					")";
				return (-1);
			}
			if (_ex < _sx || _ey < _sy) {
				cerr << "Koniec przeszkody musi byc za"
					"poczatkiem przeszkody" << endl;
				return (-1);
			}

			/*
			 * Potem na wszelki wypadek czyscimy plansze (%%)
			 */
			PlanszaXY_Czysc();

			/*
			 * I wstawiamy przeszkode w wypelnienie planszy
			 */
			for (int xi = _sx; xi <= _ex; xi++) {
				for (int yi = _sy; yi <= _ey; yi++) {
					plansza_xy[xi][yi] = '+';
				}
			}

			/* 
			 * Bez kroku (%%) okazaloby sie, ze po wielokrotnych
			 * powrotach do tego menu przeszkoda z poprzedniego
			 * ustawienia pokrywa sie z przeszkoda z obecnego
			 * ustawienia i uzyskujemy smieci na ekranie.
			 */

			/*
			 * Zapisujemy ustawienia
			 */
			przeszkoda.sx = _sx;
			przeszkoda.sy = _sy;
			przeszkoda.ex = _ex;
			przeszkoda.ey = _ey;

			return 0;
		}

		/*
		 * Czy przekazane X i Y znajduja sie w srodku przeszkody?
		 * Jezeli tak, to mamy kolizje!
		 */
		bool
		Czy_Kolizja(int kx, int ky)
		{

			if (kx >= przeszkoda.sx &&
			    kx <= przeszkoda.ex &&
			    ky >= przeszkoda.sy &&
			    ky <= przeszkoda.ey)
				return (true);
			return (false);
		}

		/*
		 * Glowny ambaras. Obsluga tego, co robi uzytkownik
		 */
		void
		Akcja(int key)
		{

			if (key == 'g') {
				/* 
				 * Od tej pory jestesmy w menu glownym;
				 * pokaz, co jest dostepne.
				 */
				gdzie = MENU_GLOWNE;
				Menu_Glowne();
			}

			if (key == '1') {
				/*
				 * Najpierw zczytaj dane dot. przeszkody;
				 * Potem pokaz Menu Glowne (jakby caly czas
				 * jestemy w glownym, tylko wczytujemy dane).
				 */
				Menu_Wprowadzania();
				Menu_Glowne();
			}
			if (key == '2') {
				/*
				 * Jestesmy w trybie "chodzenia".
				 * Narysuj plansze, narysuj menu dot.
				 * poruszania i zaznacz, ze odtad jestesmy w
				 * trybie "wczytywania klawiszy kierunkowych"
				 * z klawiatury
				 */
				Narysuj();
				Menu_Poruszania_Sie();
				gdzie = MENU_PORUSZANIA;
			}
			if (key == '3')
				/* Do zrobienia */
				cout << "Jeszcze nie dodany do programu" <<
					endl;
			if (key == '4')
				/* Wychodzimy */
				exit(0);

			/*
			 * Ktos wcisnal klawisz kierunkowy?
			 */
			if (key == UP || key == DOWN || key == LEFT ||
			    key == RIGHT) {
				/* Jestesmy w poruszaniu? */
				if (gdzie == MENU_PORUSZANIA) {
					bool jest_kolizja = false;

					/*
					 * Wymaz obecna pozycje robota --
					 * nadchodza zmiany w jego polozeniu.
					 */
					plansza_xy[robo->X()][robo->Y()] = '.';

					/* 
					 * Rusz robota.
					 * Odtad mamy nowe X i Y
					 */
					robo->Rusz(key);

					/*
					 * Dla nowych X i Y sprawdz, czy jest
					 * kolizja
					 */
					jest_kolizja = Czy_Kolizja(robo->X(), robo->Y());
					if (jest_kolizja == true) {
						/*
						 * Jest kolizja. Powroc do
						 * starych ustawien
						 */
						robo->X() = robo->oldX();
						robo->Y() = robo->oldY();
						plansza_xy[robo->X()][robo->Y()] = '.';
					} else {
						/*
						 * W nowych ustawieniach nie
						 * ma kolizji, czyli mozna
						 * bezpiecznie postawic
						 * robota. Wstaw go na plansze
						 * w nowym miejscu.
						 */
						plansza_xy[robo->X()][robo->Y()] = robo->Znak();
					}

					/*
					 * Narysuj plansze po zmianach i
					 * powtórz menu.
					 */
					Narysuj();
					Menu_Poruszania_Sie();
				}
			}
		}
};

int
main(int argc, char **argv)
{
	int r;

	initscr();

	/*
	 * Tworze robota w (0,0) z max. X = 80 i max. Y = 20.
	 * Potem tworze plansze o takich samych maksymalnych rozmiarach,
	 * i przekazuje tam robota.
	 */
	Robot *robo = new Robot('#', 0, 0, 80, 20);
	PlanszaXY *p = new PlanszaXY(80, 20, robo);

	/* Zaczynamy. */
	p->Narysuj();
	p->Menu_Glowne();
	for (;;) {
		r = getch();
		p->Akcja(r);
	}

	return (0);
}
