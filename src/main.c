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
	move2spawn(map, &player);

	// On affiche les objectifs
	displayObjectives(&lineLog, win_logs);

	// On affiche la map et le joueur
	displayFloor(map, win_game);
	displayPlayer(player, map, win_game, win_logs, &lineLog);


	/* Ici se déroule tout le jeu */
	while (continueGame) {

		clearLog(&lineLog, win_logs);
		key = getch();

		switch (key) {
			case 'q': continueGame = FALSE; break;
			case 'Q': continueGame = !askConfirmationToQuit(&lineLog, win_logs); break;



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
						default: break ;
					}
				} else  addLog("Commande invalide.", &lineLog, win_logs);
				break;
			case 'o': traiterPorte(map, player, &lineLog, win_logs); break;
			default: addLog("Commande inconnue.", &lineLog, win_logs);
		}

		markDiscoverRoom(map, player);

		displayFloor(map, win_game);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayStats(player, win_stats);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}



int askConfirmationToQuit(int * line, WINDOW * win) {
	int key;
	addLog("Etes vous sur de vouloir quitter ? (y/n) ", line, win);
	key = getch();
	switch (key) {
		case 'y': return TRUE;
		case 'n': return FALSE;
		default:
			clearLog(line, win);
			addLog("Never mind", line, win);
			return FALSE;
	}

}
