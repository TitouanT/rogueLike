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
#define NB_LVL 6

int main () {

	int key;
	int lineLog = 0;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	t_character player = {"valentin", 0, 0, 0, 10, 10, 10};


	init_screen();

	/* Initialisation des fenetres */
	WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
	WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
	WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");

	// On génère des niveaux aléatoires au nombre de
	for(int i=0;i<NB_LVL;i++){
		randomFloor(map);
		writeLvl(map,i);
	}

	// On déplace le joueur au spawn de celui-ci
	move2spawn(map, &player);

	// On affiche les objectifs
	displayObjectives(&lineLog, win_logs);

	// On affiche la map et le joueur
	displayFloor(map, win_game);
	displayPlayer(player, map, win_game, win_logs, &lineLog);


	/* Ici se déroule tout le jeu */
	while((key = getch()) != 'q' && key != 'Q'){

		clearLog(&lineLog, win_logs);

		switch (key) {
			case KEY_UP: move_perso(UP, map, &player); markDiscover(map, player);break;
			case KEY_DOWN: move_perso(DOWN, map, &player); markDiscover(map, player);break;
			case KEY_LEFT: move_perso(LEFT, map, &player); markDiscover(map, player);break;
			case KEY_RIGHT: move_perso(RIGHT, map, &player); markDiscover(map, player);break;

			case '\n':
				if(map[player.line][player.column].nbObject > 0){
					switch (map[player.line][player.column].obj[0]) {
						case STAIRS_UP: if(player.lvl<6){
																		writeLvl(map,(player.lvl));
																		(player.lvl)++;
																		readLvl(map,(player.lvl));
																		move2spawn(map, &player);
														}
																		break;
						case STAIRS_DOWN:
									if(player.lvl>0){
										writeLvl(map,(player.lvl));
										(player.lvl)-- ;
										readLvl(map,(player.lvl));
										move2Stairs_UP(map, &player);
									}
										break;

						default: addLog("Aucune raison de faire entrée ici", &lineLog, win_logs);
					}
				} else addLog("Pourquoi voulez vous faire entrée ?", &lineLog, win_logs);
				break;

			default: addLog("Commande inconnue !", &lineLog, win_logs);
		}
		
		markDiscoverRoom(map, player);

		displayFloor(map, win_game);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayStats(player, win_stats);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}
