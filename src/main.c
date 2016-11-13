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

int main () {

	int key;
	int lineLog = 0;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	t_character player = {"valentin", 0, 0, 1, 10, 10, 10};

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

	// On génère un niveau aléatoire
	randomFloor(map, FALSE);
	// On déplace le joueur au spawn de celui-ci
	move2spawn(map, &player);

	// On affiche la map et le joueur, et quelques objectifs
	displayFloor(map, win_game);
	displayPlayer(player, win_game);

	addLog("Vous venez d'apparaître au premier étage !", &lineLog, win_logs);
	addLog(" > Allez sauver Nathalie Camelin", &lineLog, win_logs);
	addLog(" > Evitez de vous faire attraper par des L1", &lineLog, win_logs);


	/* Ici se déroule tout le jeu */
	while((key = getch()) != 'q' && key != 'Q'){

		clearLog(&lineLog, win_logs);

		switch (key) {
			case KEY_UP: move_perso(UP, map, &player); break;
			case KEY_DOWN: move_perso(DOWN, map, &player); break;
			case KEY_LEFT: move_perso(LEFT, map, &player); break;
			case KEY_RIGHT: move_perso(RIGHT, map, &player); break;

			case '\n':
				if(map[player.line][player.column].nbObject > 0){
					switch (map[player.line][player.column].obj[0]) {
						case STAIRS_UP: randomFloor(map, FALSE); move2spawn(map, &player); (player.lvl)++; break;
						case STAIRS_DOWN: addLog("vous êtes déjà en bas !", &lineLog, win_logs); break;
						default: addLog("Aucune raison de faire entrée ici", &lineLog, win_logs);
					}
				} else addLog("Pourquoi voulez vous faire entrée ?", &lineLog, win_logs);
				break;

			default: addLog("Commande inconnue !", &lineLog, win_logs);
		}

		// TEST POUR VOIR SI LE SPAWN EST BIEN MIS A JOUR
		//randomFloor(map, FALSE);
		//move2spawn(map, &player);
		// FIN DU GAME ... EUH DU TEST


		displayFloor(map, win_game);
		displayPlayer(player, win_game);
		displayStats(player, win_stats);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}
