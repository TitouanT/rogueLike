#include "global.h"

int main () {

	int key;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];

	initscr();

	/* Initialisation des panels et des fenetres */
	WINDOW *windows[3];
	PANEL  *panels[3];

	windows[0] = newwin(LINES_GAME, COLS_GAME, 0, 0);
	windows[1] = newwin(LINES_STATS, COLS_STATS, LINES_GAME, 0);
	windows[2] = newwin(LINES_LOGS, COLS_LOGS, 0, COLS_GAME);

	/* On trace les boxes autour des ces fenetres */
	box(windows[0], 0, 0);
	box(windows[1], 0, 0);
	box(windows[2], 0, 0);

	/* On affecte à un panel chaque fenetres */
	panels[0] = new_panel(windows[0]);
	panels[1] = new_panel(windows[1]);
	panels[2] = new_panel(windows[2]);


	update_panels();
	doupdate();

	mvprintw(0,2, "RogueLike");
	mvprintw(LINES_GAME,2, "Statistiques");
	refresh();
	mvprintw(0, COLS_GAME+2, "Logs");

	// On déplace le curseur à la fin
	move(LINES_GAME + LINES_STATS, 0);

	keypad(stdscr, TRUE);

	while((key = getch()) != 'q'){
		randomFloor(map, FALSE);
		displayFloor(map);
	}

	endwin();

	return 0;
}
