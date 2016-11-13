#include "display.h"


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
			switch (map[i][j].type) {
				case EMPTY: 	 printCell(1,' ', win); break;
				case DOORWAY:
					switch (map[i][j].state) {
						case dNONE:  printCell(2,'c', win); break;
						case dOPEN:  printCell(8,'c', win); break;
						case dCLOSE: printCell(1,'c', win); break;
						default: printCell(1,'?', win); break;
					}
					break;
				case ROOM:
					if (map[i][j].nbObject == 0) printCell(4,' ', win);
					else {
						switch (map[i][j].obj[0]) {
							case STAIRS_UP: printCell(7,'<', win); break;
							case STAIRS_DOWN: printCell(7, '>', win); break;
							case objNONE: printCell(4,' ', win); break;
						}
					}
					break;

				case CORRIDOR:
					if (map[i][j].nbObject == 0) printCell(2,'c', win);
					else {
						switch (map[i][j].obj[0]) {
							case STAIRS_UP: printCell(7,'<', win); break;
							case STAIRS_DOWN: printCell(7, '>', win); break;
							case objNONE: printCell(2,' ', win); break;
						}
					}
					break;

				case WALL: 		 printCell(3,'c', win); break;
			}
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

	wattron(win, COLOR_PAIR(1));

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
		wattron(win, COLOR_PAIR(6));
	}
	else wattron(win, COLOR_PAIR(9));

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
	wprintw(win, "Joueur : %i %i  |  Etage : %i",player.line, player.column, player.lvl);
	wrefresh(win);

}
