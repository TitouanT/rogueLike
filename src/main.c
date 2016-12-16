/*! \mainpage rogueLike documentation
 *
 * Dans le cadre de notre projet en L2 SPI à l'université du Maine, nous avons réalisé un rogueLike. Le code se trouve à l'adresse suivante : <a href="https://github.com/TitouanT/rogueLike">https://github.com/TitouanT/rogueLike</a>
 * <img src="../include/screen_doxygen.png" style="width:100%;">
 * \section requirements_sec Programmes requis
 *
 * Pour utiliser notre jeu, vous aurez besoin de :
 *
 * - Compilateur `gcc`
 * - Constructeur `make`
 * - Librairie `ncurses`
 * - Gestionnaire `git`
 *
 *
 * \section installation_sec Installation
 *
 * Pour installer notre rogueLike, ouvrez un terminal, et entrez les commandes suivantes :
 *
 * \code{shell}
 * git clone https://github.com/TitouanT/rogueLike.git
 * cd rogueLike
 * make install
 * \endcode
 *
 *
 * \section lancement_sec Lancement du jeu
 *
 * \code{shell}
 * ./rogueLike
 * \endcode
 *
 *
 * \section jouer_sec Comment jouer ?
 *
 * - Pour se déplacer, utilisez les flèches de direction.
 * - Pour ouvrir une porte rouge, appuyez sur la touche `o` et signalez la direction de la porte avec les flèches de direction.
 * - Pour fermer une porte verte, appuyez sur la touche `f` et signalez la direction de la porte avec les flèches de direction.
 * - Pour passer à l'étage suivant ou à l'étage précédent, placez vous sur un escalier et appuyez sur la touche "entrée".
 * - Pour voir votre inventaire, appuyez sur la touche `i`.
 * - Pour récupérer un objet, placez-vous dessus, puis appuyez sur la touche `g`. Vous devez avoir un emplacement de libre dans votre inventaire.
 * - Pour lâcher un objet, appuyez sur la touche `d`, et indiquez le numéro de l'emplacement de l'objet que vous voulez lâcher.
 * - Pour sauvegarder le jeu, appuyez sur `s`.
 * - Pour quitter le jeu, appuyez sur `q`.
 * - Vous pouvez obtenir de l'aide en jeu en appuyant sur la touche `?`.
 *
 *
 */

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

int main () {

	if (fileExist("./accentNeeded")) setlocale(LC_ALL, "");

	initErr();

	err ("\n***DEBUT DU MAIN***\n");

	int key;
	int i, j;
	int lineLog;
	int widthScreen, heightScreen;
	int continueGame = TRUE;
	int nbMonster;
	int visibleByGhost[LINES][COLUMNS];
	int isPlayerInvicible;
	int wasNewGame;
	t_cell map[LINES][COLUMNS];
	t_monster monsters[NB_MONSTER_MAX];
	t_character player;


	err ("  main: initialisations");

	initRandom();

	init_screen();
	getmaxyx(stdscr,heightScreen,widthScreen);

	err ("  main: Affichage du screen de départ");
	WINDOW *win_start = newwin(heightScreen, widthScreen, 0, 0);
	startScreen(win_start);

	deleteWindow(win_start);


	do{
		// Mise à zéro de toutes les variables, utile lorsque le joueur relance une partie
		player = (t_character){"root", 0, 0, 0, 10, 10, 60, 10, 0, MAX_FOOD, FALSE, FALSE, {objNONE}};
		lineLog = 0;
		isPlayerInvicible = FALSE;
		for(i = 0 ; i < LINES ; i++){
			for(j = 0 ; j < COLUMNS ; j++){
				visibleByGhost[i][j] = 0;
			}
		}

		err ("  main: Affichage du screen de choix");
		WINDOW *win_choice = newwin(heightScreen, widthScreen, 0, 0);
		wasNewGame = selectionScreen(win_choice, map, &player, monsters, &nbMonster);

		deleteWindow(win_choice);

		err ("  main: Affichage du screen avec map, logs et stats");
		/* Initialisation des fenetres */
		WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
		WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
		WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");

		// On affiche les objectifs
		if(wasNewGame) displayObjectives(&lineLog, win_logs);
		// On affiche la map et le joueur
		displayFloor(map, player, win_game, visibleByGhost);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayMonster (win_game, monsters, map, nbMonster, player.lvl, visibleByGhost);
		displayStats(player, win_stats, isPlayerInvicible);


		err ("\nmain***ENTREE DANS LA BOUCLE DU JEU***\n");

		/* Ici se déroule tout le jeu */

		while (continueGame && player.hp > 0) {

			err ("\n  main*** debut d'un tour de jeu ***");
			key = getch();
			if(isPlayerInvicible){
				player.hp   = 10000;
				player.food = 10000;
			}

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
			continueGame = handleInteraction(key, map, &player, win_logs, win_game, &lineLog, monsters, nbMonster, &isPlayerInvicible);
			err("  main: recuperation apres handle interaction");
			markDiscoverRoom(map, player);

			//moveMonster(map, monsters, nbMonster, &player);
			setVisibleByGhost (monsters, visibleByGhost, player);

			err("  main: affichage etage, player, stats");
			displayFloor(map, player, win_game, visibleByGhost);
			displayPlayer(player, map, win_game, win_logs, &lineLog);
			displayMonster (win_game, monsters, map, nbMonster, player.lvl, visibleByGhost);
			displayStats(player, win_stats, isPlayerInvicible);
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
			while((key = getch()) != 'q' && key != 'Q' && up2low(key) != 'r');
			err ("main*** Fin Affichage du gameOver ***");
		}
	}while(key != 'q' && key != 'Q');

	endwin(); //Fermeture de la fenetre
	err ("\n***FIN DU MAIN***\n");
	unInitErr();
	return EXIT_SUCCESS;

}
