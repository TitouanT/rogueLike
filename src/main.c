/**
 * \file main.c
 * \brief Programme principal du jeu.
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 0.1
 * \date 12 novembre 2016
 */

#include "global.h"

int main () {

	int key;
	int lineLog = 0;
	int widthScreen, heightScreen;
	int continueGame = TRUE;

	initRandom();
	t_cell map[LINES][COLUMNS];
	t_character player = {"valentin", 0, 0, 0, 9, 10, 10, 0};


	init_screen();
	getmaxyx(stdscr,heightScreen,widthScreen);

	WINDOW *win_start = newwin(heightScreen, widthScreen, 0, 0);
	startScreen(win_start);

	deleteWindow(win_start);

	WINDOW *win_choice = newwin(heightScreen, widthScreen, 0, 0);
	selectionScreen(win_choice);

	deleteWindow(win_choice);

	/* Initialisation des fenetres */
	WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
	WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
	WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");



	// Initialisation des niveaux
	initGameMap(map,0,0);
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


		if(key=='T'){key=getch();if(key=='o'){key=getch();if(key=='W'){key=getch();if(key=='i'){key=getch();if(key=='n'){key=getch();if(key=='I'){key=getch();if(key=='C'){key=getch();if(key=='h'){key=getch();if(key=='e'){key=getch();if(key=='a'){key=getch();if(key=='t'){displayFloorCheat(map, win_game);getch();}}}}}}}}}}}

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
