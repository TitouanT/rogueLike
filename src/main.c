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

	/* Affichage des titres sur les panels */
	mvprintw(0,2, "RogueLike");
	mvprintw(LINES_GAME,2, "Statistiques");
	refresh();
	mvprintw(0, COLS_GAME+2, "Logs");


	move(LINES_GAME + LINES_STATS, 0); // On déplace le curseur à la fin

	keypad(stdscr, TRUE); // Pour ne pas afficher les lettres que l'utilisateur tape
	noecho();
	curs_set(0);


	/* Ici se déroule tout le jeu */
	while((key = getch()) != 'q'){
		clearLog(&lineLog);

		randomFloor(map, FALSE);
		displayFloor(map);
		addLog("Vous avez éffecuté une action", &lineLog);

		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		if(key == '\n') addLog("Attention, la touche entrée n'est pas faite pour manger le dessert !", &lineLog);
		else addLog("Erreur", &lineLog);

		addLog("fdsjflkjsdljflsdfljsdlfkslf", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);
		addLog("azertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBNazertyuiopqsdfghjklmwxcvbn0123456789AZERTYUIOPQSDFGHJKLMWXCVBN", &lineLog);

		addLog("HEEEY", &lineLog);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}
