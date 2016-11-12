#include "display.h"


/* Cette fonction crée une fenetre aux coordonnées indiquée, et de taille donnée */
WINDOW *createWindow(int startX, int startY, int width, int height, char * label){

	WINDOW *localWindow;

	localWindow = newwin(height, width, startY, startX);

	start_color();
	// Init pair : init_pair(ID_PAIR, TEXT COLOR, BACKGROUND COLOR);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
	init_pair(5, COLOR_BLACK, COLOR_BLACK);

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
		case ' ': wprintw(win, " "); break;
		case 'c': waddch(win, ACS_CKBOARD); break;
	}
}

/* Cette fonction affiche l'étage de la map donnée en paramètre */
void displayFloor (t_cell map[][COLUMNS], WINDOW *win) {

	int i, j;

	for (i = 0; i < LINES; i++) {
		wmove(win, i+1,1);
		for (j = 0; j < COLUMNS; j++) {
			switch (map[i][j].type) {
				case EMPTY: 	 printCell(1,' ', win); break;
				case DOOR: 		 printCell(1,'c', win); break;
				case ROOM: 		 printCell(4,' ', win); break;
				case CORRIDOR: printCell(2,'c', win); break;
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
