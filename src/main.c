/**
 * \file
 * \brief Programme principal du jeu.
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.2
 * \date 12 novembre 2016
 */

#include "global.h"

int main () {
	remove("err");
	int key;
	int lineLog = 0;
	int widthScreen, heightScreen;
	int continueGame = TRUE;

	initRandom();
	t_cell map[LINES][COLUMNS];
	t_character player = {"valentin", 0, 0, 0, 10, 10, 10, 0, MAX_FOOD, FALSE, FALSE};

	init_screen();
	getmaxyx(stdscr,heightScreen,widthScreen);

	WINDOW *win_start = newwin(heightScreen, widthScreen, 0, 0);
	startScreen(win_start);

	deleteWindow(win_start);

	WINDOW *win_choice = newwin(heightScreen, widthScreen, 0, 0);
	selectionScreen(win_choice, map, &player);

	deleteWindow(win_choice);

	/* Initialisation des fenetres */
	WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
	WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
	WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");

	// On affiche les objectifs
	displayObjectives(&lineLog, win_logs);
	// On affiche la map et le joueur
	displayFloor(map, win_game);
	displayPlayer(player, map, win_game, win_logs, &lineLog);
	displayStats(player, win_stats);



	/* Ici se déroule tout le jeu */
	while (continueGame && player.hp > 0) {

		key = getch();
		if (key == 'N') {
			randomFloor(map, 6);
			displayFloor(map, win_game);
			displayPlayer(player, map, win_game, win_logs, &lineLog);
		}

		clearLog(&lineLog, win_logs);
		continueGame = handleInteraction(key, map, &player, win_logs, win_game, &lineLog);

		markDiscoverRoom(map, player);

		displayFloor(map, win_game);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayStats(player, win_stats);

	}

	// ----- UNE FOIS QUE LE JEU EST TERMINE ----- //

	// Si le joueur est sorti du jeu sans vouloir quitter manuellement
	if(key != 'q' && key != 'Q'){
		WINDOW *win_end = newwin(heightScreen, widthScreen, 0, 0);
		displayEnd(player, win_end);
		while((key = getch()) != 'q' && key != 'Q');
	}


	endwin(); //Fermeture de la fenetre
	return 0;

}
