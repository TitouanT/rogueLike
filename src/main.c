/*
 * \file main.c
 * \brief Programme principal du jeud.
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 0.1
 * \date 12 novembre 2016
 */

#include "global.h"
int askConfirmationToQuit(int * line, WINDOW * win);

int main () {

	int key;
	int lineLog = 0;
	int continueGame = TRUE;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	t_character player = {"bibi", 0, 0, 0, 9, 10, 10, 0};


	init_screen();

	/* Initialisation des fenetres */
	WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
	WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
	WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");



	// On génère des niveaux aléatoires au nombre de
	InitGameMap(map);
	// On déplace le joueur au spawn de celui-ci
	move2spawn(map, &player, STAIRS_DOWN);
	// On affiche les objectifs
	displayObjectives(&lineLog, win_logs);
	// On affiche la map et le joueur
	displayFloor(map, win_game);
	displayPlayer(player, map, win_game, win_logs, &lineLog);
	displayStats(player, win_stats);





	/* Ici se déroule tout le jeu */
	while (continueGame) {

		key = getch();
		clearLog(&lineLog, win_logs);
		continueGame = handleInteraction(key, map, &player, win_logs, &lineLog);

		markDiscoverRoom(map, player);

		displayFloor(map, win_game);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayStats(player, win_stats);

	}




	endwin(); //Fermeture de la fenetre
	return 0;
}
