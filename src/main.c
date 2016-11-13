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

void colorFinder (t_cell map[][COLUMNS], t_character player, int color, WINDOW * win_mess, WINDOW * win_game, WINDOW * win_log, int * line);

int main () {

	int key;
	int lineLog = 0;

	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	t_character player = {"valentin", 0, 0, 1, 10, 10, 10};

	initscr();
	start_color();
	init_color(COLOR_DOOR_OPEN, 0, 500, 0);

	init_color(COLOR_EMPTY, 0, 0, 0);
	init_color(COLOR_LIGHT, 1000, 1000, 1000);
	init_color(COLOR_FLOOR, 260, 260, 0);//++++++
	init_color(COLOR_WALL, 300, 60, 0);
	init_color(COLOR_PLAYER_B, 0, 0, 0);
	init_color(COLOR_PLAYER_F, 1000, 1000, 1000);

	// Init pair : init_pair(ID_PAIR, TEXT COLOR, BACKGROUND COLOR);
	// init_pair(1, COLOR_RED, COLOR_BLACK); // log(t), closed door(t), empty
	// init_pair(2, COLOR_WHITE, COLOR_BLACK); // corridor(t)
	// init_pair(3, COLOR_CYAN, COLOR_BLACK); // wall(t)
	// init_pair(4, COLOR_WHITE, COLOR_WHITE); // room
	// init_pair(5, COLOR_BLACK, COLOR_BLACK); // ?
	// init_pair(6, COLOR_GREEN, COLOR_WHITE); // player (t)
	// init_pair(7, COLOR_BLACK, COLOR_WHITE); // stairs (t)
	// init_pair(8, COLOR_GREEN, COLOR_BLACK); // opended door(t)

	init_pair (1, COLOR_DOOR_CLOSE, COLOR_EMPTY);//door close
	init_pair (2, COLOR_FLOOR, COLOR_EMPTY);//corridor
	init_pair (3, COLOR_WALL, COLOR_EMPTY);//wall
	init_pair (4, COLOR_FLOOR, COLOR_FLOOR);//rooom
	init_pair (5, COLOR_BLACK, COLOR_BLACK); // ?
	init_pair (6, COLOR_PLAYER_F, COLOR_PLAYER_B);//player
	init_pair (7, COLOR_EMPTY, COLOR_FLOOR); // stairs (t)
	init_pair (8, COLOR_DOOR_OPEN, COLOR_EMPTY);//door open

	init_pair(9, 10, 8); // 8 : dark grey


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
	addLog("Vous venez d'apparaître au premier étage !", &lineLog, win_logs);
	addLog(" > Allez sauver Nathalie Camelin", &lineLog, win_logs);
	addLog(" > Evitez de vous faire attraper par des L1", &lineLog, win_logs);

	displayFloor(map, win_game);
	displayPlayer(player, map, win_game, win_logs, &lineLog);


	/* Ici se déroule tout le jeu */
	while((key = getch()) != 'q' && key != 'Q'){

		clearLog(&lineLog, win_logs);

		switch (key) {
			case 'c':
				addLog ("Bienvenu dans le mode couleur !!",&lineLog, win_logs);
				addLog ("1: color of floor",&lineLog, win_logs);
				addLog ("2: color of wall",&lineLog, win_logs);
				addLog ("3: color of player(f)",&lineLog, win_logs);
				addLog ("4: color of player(b)",&lineLog, win_logs);
				while ((key = getch()) != 'q') {
					switch(key) {
						case '1': colorFinder(map, player, COLOR_FLOOR, win_stats, win_game, win_logs, &lineLog); break;
						case '2': colorFinder(map, player, COLOR_WALL, win_stats, win_game, win_logs, &lineLog); break;
						case '3': colorFinder(map, player, COLOR_PLAYER_F, win_stats, win_game, win_logs, &lineLog); break;
						case '4': colorFinder(map, player, COLOR_PLAYER_B, win_stats, win_game, win_logs, &lineLog); break;
					}

				}
				clearLog(&lineLog, win_logs);
				break;

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

		displayFloor(map, win_game);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayStats(player, win_stats);

	}


	endwin(); //Fermeture de la fenetre
	return 0;
}

void colorFinder (t_cell map[][COLUMNS], t_character player, int color, WINDOW * win_mess, WINDOW * win_game, WINDOW * win_log, int * line) {
	int key, r, g, b;
	r=g=b=0;
	mvwprintw(win_mess, 1,1,"Vous aller modifier une couleur !!");
	wrefresh(win_mess);
	while((key = getch()) != '\n'){
		mvwprintw(win_mess, 1,1,"                                  ");

		switch (key) {
			case 'f': if (r+10<=1000) r+=10; break;
			case 'g': if (g+10<=1000) g+=10; break;
			case 'h': if (b+10<=1000) b+=10; break;
			case 'c': if (r-10>=0) r-=10; break;
			case 'v': if (g-10>=0) g-=10; break;
			case 'b': if (b-10>=0) b-=10; break;
		}
		init_color(color, r, g, b);


		mvwprintw(win_mess, 1,1,"r: %d, g: %d, b: %d", r,g,b);
		displayFloor(map, win_game);
		displayPlayer(player, map, win_game, win_log, line);
		wrefresh(win_mess);


	}
}
