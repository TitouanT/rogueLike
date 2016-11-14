#include "display.h"
#include "global.h"



void init_colors(){

	start_color();

	init_pair(GENERAL_COLOR , COLOR_RED,   COLOR_BLACK);
	init_pair(CORRIDOR_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(WALL_COLOR    , COLOR_CYAN,  COLOR_BLACK);
	init_pair(ROOM_COLOR    , COLOR_WHITE, COLOR_WHITE);
	init_pair(PLAYER_COLOR  , COLOR_GREEN, COLOR_WHITE);
	init_pair(OBJECTS_COLOR , COLOR_BLACK, COLOR_WHITE);
	init_pair(OPENED_DOOR   , COLOR_WHITE, COLOR_GREEN);
	init_pair(PLAYER_C_COLOR, COLOR_GREEN, COLOR_BLACK);
}

void init_screen(){
	initscr();
	init_colors();

	keypad(stdscr, TRUE); // Pour ne pas afficher les lettres que l'utilisateur tape
	noecho();
	curs_set(0);
	refresh();
}

void displayObjectives(int *lineLog, WINDOW *win_logs){

	addLog("Vous venez d'apparaître au premier étage !", lineLog, win_logs);
	addLog(" > Allez sauver Nathalie Camelin", lineLog, win_logs);
	addLog(" > Evitez de vous faire attraper par des L1", lineLog, win_logs);

}

/* Cette fonction crée une fenetre aux coordonnées indiquée, et de taille donnée */
WINDOW *createWindow(int startX, int startY, int width, int height, char * label){

	WINDOW *localWindow;

	localWindow = newwin(height, width, startY, startX);


	box(localWindow, 0, 0);
	wmove(localWindow,0,2);
	wprintw(localWindow, label);

	wrefresh(localWindow);

	return localWindow;

}

void gotoEndGame(){
	move(LINES_STATS + LINES_GAME, 0); //On déplace le curseur à la fin
}

/* Cette fonction affiche le contenu d'une cellule (' ' ou 'c') avec la paire de couleur choisie */
void printCell(int pair, char cell, WINDOW *win){

	wattron(win, COLOR_PAIR(pair));
	switch (cell) {
		case 'c': waddch(win, ACS_CKBOARD); break;
		default: wprintw(win, "%c", cell); break;
	}
}

/* Cette fonction affiche l'étage de la map donnée en paramètre */
void displayFloor (t_cell map[LINES][COLUMNS], WINDOW *win) {

	int i, j;

	for (i = 0; i < LINES; i++) {
		wmove(win, i+1,1);
		for (j = 0; j < COLUMNS; j++) {
			if (map[i][j].isDiscovered) {
				switch (map[i][j].type) {
					case EMPTY: 	 printCell(GENERAL_COLOR,' ', win); break;
					case DOORWAY:
						switch (map[i][j].state) {
							case dNONE:  printCell(CORRIDOR_COLOR,'c', win); break;
							case dOPEN:  printCell(OPENED_DOOR,'c', win); break;
							case dCLOSE: printCell(GENERAL_COLOR,'c', win); break;
							default: printCell(GENERAL_COLOR,'?', win); break;
						}
						break;
					case ROOM:
						if (map[i][j].nbObject == 0) printCell(ROOM_COLOR,' ', win);
						else {
							switch (map[i][j].obj[0]) {
								case STAIRS_UP: printCell(OBJECTS_COLOR,'<', win); break;
								case STAIRS_DOWN: printCell(OBJECTS_COLOR, '>', win); break;
								case objNONE: printCell(ROOM_COLOR,' ', win); break;
							}
						}
						break;

					case CORRIDOR:
						if (map[i][j].nbObject == 0) printCell(CORRIDOR_COLOR,'c', win);
						else {
							switch (map[i][j].obj[0]) {
								case STAIRS_UP: printCell(OBJECTS_COLOR,'<', win); break;
								case STAIRS_DOWN: printCell(OBJECTS_COLOR, '>', win); break;
								case objNONE: printCell(CORRIDOR_COLOR,' ', win); break;
							}
						}
						break;

					case WALL: 		 printCell(WALL_COLOR,'c', win); break;
				}
			}
			else printCell(GENERAL_COLOR,' ', win); //EMPTY
		}

	}
	wrefresh(win);
	gotoEndGame();
}

/* Cette fonction efface la fenetre de log */
void clearLog(int *line, WINDOW *win){

	int i, j;

	for(i = 1 ; i <= LINES_LOGS - 2 ; i++){
		for(j = 1 ; j <= COLS_LOGS - 2 ; j++){
			wmove(win, i, j);
			wprintw(win, " ");
		}
	}
	wrefresh(win);
	*line = 0;
	gotoEndGame();
}

/* Cette fonction ajoute une ligne à la fenetre de log */
void addLog(char * message, int * line, WINDOW *win){

	wattron(win, COLOR_PAIR(GENERAL_COLOR));

	// On découpe le message en sous messages pour rentrer dans la zone de logs
	while(strlen(message) > COLS_LOGS-1) {

		wmove(win, (*line)+1, 1);
  	wprintw(win, "%.*s", COLS_LOGS-2, message);

		(*line)++;
    message+=COLS_LOGS-2;
  }
	wmove(win, (*line)+1, 1);
  wprintw(win, "%s", message);
	wrefresh(win);

	// Si on a plus de place pour clear la zone de logs
	if(*line >= LINES_LOGS - 3) clearLog(line, win);
	else (*line)++;
	gotoEndGame();
}

/* Cette fonction affiche le joueur sur le jeu */
void displayPlayer(t_character player, t_cell mat[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line){

	if(mat[player.line][player.column].type == ROOM){
		wattron(win, COLOR_PAIR(PLAYER_COLOR));
	}
	else wattron(win, COLOR_PAIR(PLAYER_C_COLOR));

	wmove(win, (player.line)+1, (player.column)+1);
	wprintw(win, "@");
	wrefresh(win);

	if(mat[player.line][player.column].nbObject != 0){

		switch (mat[player.line][player.column].obj[0]) {
			case STAIRS_UP: addLog("Vous pouvez monter les escaliers avec :           > Entrée", line, logs); break;
			case STAIRS_DOWN: addLog("Vous pouvez déscendre les éscaliers avec :      > Entrée", line, logs); break;
			default: break;
		}

	}

}


void printBar(int value, int max, int color1, int color2, WINDOW * win){

	int i;



	init_pair(BAR_TMP_1, color1, color1);
	init_pair(BAR_TMP_2, color2, color2);


	wattron(win, COLOR_PAIR(BAR_TMP_1));

	for(i = 0 ; i < value ; i++){
		wprintw(win, " ");
	}


	wattron(win, COLOR_PAIR(BAR_TMP_2));

	for(i = value ; i < max ; i++){
		wprintw(win, " ");
	}

	wrefresh(win);
	wattroff(win, COLOR_PAIR(BAR_TMP_2));

}

void displayStats(t_character player, WINDOW *win){

	int i, j;

	// On clear la zone de stats
	for(i = 1 ; i < LINES_STATS -1; i++){
		for(j = 1 ; j < COLS_STATS -1; j++){
			wmove(win, i, j);
			wprintw(win, " ");
		}
	}

	wmove(win, 1, 1);
	wprintw(win, "Etage     : ");
	printBar(player.lvl, NB_LVL, COLOR_YELLOW, COLOR_CYAN, win);

	wmove(win, 2, 1);
	wprintw(win, "HP        : ");
	printBar(player.hp, MAX_HP, COLOR_GREEN, COLOR_RED, win);

	wmove(win, 3, 1);
	wprintw(win, "Puissance : %i", player.pw);

	wmove(win, 4, 1);
	wprintw(win, "XP        : %i", player.xp);
	
	wrefresh(win);

}
