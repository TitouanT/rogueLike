#include "global.h"

int main () {

	int key;
	int lineLog = 0;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	t_character player = {"valentin", 0, 0, 10, 10, 10, 10};

	initscr();
	start_color();

	keypad(stdscr, TRUE); // Pour ne pas afficher les lettres que l'utilisateur tape
	noecho();
	curs_set(0);
	refresh();

	/* Initialisation des fenetres */
	WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
	WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
	WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");


	randomFloor(map, FALSE);

	displayFloor(map, win_game);
	displayPlayer(player, win_game);

	/* Ici se déroule tout le jeu */
	while((key = getch()) != 'q' && key != 'Q'){

		clearLog(&lineLog, win_logs);

		switch (key) {
			case KEY_UP: addLog("Vous avez bougé vers le haut", &lineLog, win_logs); (player.line)--; break;
			case KEY_DOWN: addLog("Vous avez bougé vers le bas", &lineLog, win_logs); (player.line)++; break;
			case KEY_LEFT: addLog("Vous avez bougé vers la gauche", &lineLog, win_logs); (player.column)--; break;
			case KEY_RIGHT: addLog("Vous avez bougé vers la droite", &lineLog, win_logs); (player.column)++; break;
			case '\n': addLog("Entrée pas faite.", &lineLog, win_logs); break;
			default: addLog("Commande inconnue !", &lineLog, win_logs);
		}


		displayFloor(map, win_game);
		displayPlayer(player, win_game);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}
