/**
 * \file
 * \brief Programme principal du jeu.
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.2
 * \date 12 novembre 2016
 */
#include <locale.h>       // OK
#include <ncurses.h>      // OK
#include <stdlib.h>       // OK

#include "display.h"      // OK
#include "cell.h"         // OK
#include "character.h"    // OK
#include "lvl.h"          // OK
#include "mapConst.h"     // OK
#include "moves.h"        // OK
#include "interactions.h" // OK
#include "tools.h"        // OK
#include "monstre.h"      // OK

#include "loadLvl.h"

int main () {

	if (fileExist("./accentNeeded")) setlocale(LC_ALL, "");

	initErr();

	err ("\n***DEBUT DU MAIN***\n");

	int key;
	int lineLog = 0;
	int widthScreen, heightScreen;
	int continueGame = TRUE;
	int nbMonster = 0;


	err ("  main: initialisations");

	initRandom();
	t_cell map[LINES][COLUMNS];
	t_monster monsters[NB_MONSTER_MAX];
	int visibleByGhost[LINES][COLUMNS] = {{0}};
	t_character player = {"root", 0, 0, 0, 10, 10, 60, 10, 0, MAX_FOOD, FALSE, FALSE, {objNONE}};

	init_screen();
	getmaxyx(stdscr,heightScreen,widthScreen);

	err ("  main: Affichage du screen de départ");
	WINDOW *win_start = newwin(heightScreen, widthScreen, 0, 0);
	startScreen(win_start);

	deleteWindow(win_start);

	// err ("  main: Affichage du screen de choix");
	// WINDOW *win_choice = newwin(heightScreen, widthScreen, 0, 0);
	// selectionScreen(win_choice, map, &player, monsters, &nbMonster);
	// 
	// deleteWindow(win_choice);

	err ("  main: Affichage du screen avec map, logs et stats");
	/* Initialisation des fenetres */
	WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
	WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
	WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");
	
	updateLogs (lineLog, win_logs, win_game);
	initGameMap (map, NEW_GAME, 1, &player, monsters, &nbMonster);

	// On affiche les objectifs
	//displayObjectives(&lineLog, win_logs);
	// On affiche la map et le joueur
	displayFloor(map, player, win_game, visibleByGhost);
	//displayPlayer(player, map, win_game, win_logs, &lineLog);
	//displayMonster (win_game, monsters, map, nbMonster, player.lvl, visibleByGhost);
	//displayStats(player, win_stats);



	err ("\nmain***ENTREE DANS LA BOUCLE DU JEU***\n");

	/* Ici se déroule tout le jeu */

	while (continueGame && player.hp > 0) {

		err ("\n  main*** debut d'un tour de jeu ***");
		key = getch();

		if (key == 'N') {
			err("    main: cheat N pour une nouvelle map");
			randomFloor(map, 6);
			displayFloor(map, player, win_game, visibleByGhost);
			move2spawn(map, &player, STAIRS_DOWN);
			displayPlayer(player, map, win_game, win_logs, &lineLog);
			err("    main: fin du cheat N");
		}

		clearLog(&lineLog, win_logs);
		err("  main: passe la main a handle interaction");
		continueGame = handleInteraction(key, map, &player, win_logs, win_game, &lineLog, monsters, nbMonster);
		err("  main: recuperation apres handle interaction");
		markDiscoverRoom(map, player);

		//moveMonster(map, monsters, nbMonster, &player);
		setVisibleByGhost (monsters, visibleByGhost, player);

		err("  main: affichage etage, player, stats");
		displayFloor(map, player, win_game, visibleByGhost);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayMonster (win_game, monsters, map, nbMonster, player.lvl, visibleByGhost);
		displayStats(player, win_stats);
		err("  main: fin affichage etage, player, stats");

		err ("main*** fin d'un tour de jeu ***\n");

	}
	err ("\nmain***SORTIE DE LA BOUCLE DU JEU***\n");



	// ----- UNE FOIS QUE LE JEU EST TERMINE ----- //

	// Si le joueur est sorti du jeu sans vouloir quitter manuellement
	if(key != 'q' && key != 'Q'){
		err ("main*** Affichage du gameOver ***");
		WINDOW *win_end = newwin(heightScreen, widthScreen, 0, 0);
		displayEnd(player, win_end);
		while((key = getch()) != 'q' && key != 'Q');
		err ("main*** Fin Affichage du gameOver ***");
		main();
	}


	endwin(); //Fermeture de la fenetre
	err ("\n***FIN DU MAIN***\n");
	unInitErr();
	return EXIT_SUCCESS;

}
