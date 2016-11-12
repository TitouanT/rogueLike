#include "global.h"

int main () {

	int key;
	int lineLog = 0;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];

	initscr();
	start_color();

	// Init pair : init_pair(ID_PAIR, TEXT COLOR, BACKGROUND COLOR);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
	init_pair(5, COLOR_BLACK, COLOR_BLACK);

	/* Initialisation des fenetres */
	WINDOW *windows[3];

	windows[0] = newwin(LINES_GAME, COLS_GAME, 0, 0);
	windows[1] = newwin(LINES_STATS, COLS_STATS, LINES_GAME, 0);
	windows[2] = newwin(LINES_LOGS, COLS_LOGS, 0, COLS_GAME);


	keypad(stdscr, TRUE); // Pour ne pas afficher les lettres que l'utilisateur tape
	noecho();
	curs_set(0);
	refresh();


	/* On trace les boxes autour des ces fenetres */
	box(windows[0], 0, 0);
	box(windows[1], 0, 0);
	box(windows[2], 0, 0);

	/* On affiche le titre de chaque fenetre */
	wmove(windows[0],0,2);
	wprintw(windows[0], "RogueLike");

	wmove(windows[1],0,2);
	wprintw(windows[1], "Statistiques");

	wmove(windows[2],0,2);
	wprintw(windows[2], "Logs");

	/* On refresh les windows */
	wrefresh(windows[0]);
	wrefresh(windows[1]);
	wrefresh(windows[2]);


	randomFloor(map, FALSE);
	displayFloor(map);


	/* Ici se déroule tout le jeu */
	while((key = getch()) != 'q'){

		clearLog(&lineLog);

		switch (key) {
			case KEY_UP: addLog("Vous avez bougé vers le haut", &lineLog); break;
			case KEY_DOWN: addLog("Vous avez bougé vers le bas", &lineLog); break;
			case KEY_LEFT: addLog("Vous avez bougé vers la gauche", &lineLog); break;
			case KEY_RIGHT: addLog("Vous avez bougé vers la droite", &lineLog); break;
		}


		displayFloor(map);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}
