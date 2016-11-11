#include "display.h"


void gotoEndGame(){
	move(LINES_STATS + LINES_GAME, 0); //On déplace le curseur à la fin
}

/* Cette fonction affiche le contenu d'une cellule (' ' ou 'c') avec la paire de couleur choisie */
void printCell(int pair, char cell){


	attron(COLOR_PAIR(pair));

	switch (cell) {
		case ' ': printw(" "); break;
		case 'c': addch(ACS_CKBOARD); break;
	}
}

/* Cette fonction affiche l'étage de la map donnée en paramètre */
void displayFloor (t_cell map[][COLUMNS]) {

	int i, j;

	for (i = 0; i < LINES; i++) {
		move(i+1,1);
		for (j = 0; j < COLUMNS; j++) {
			switch (map[i][j].type) {
				case EMPTY: 	 printCell(1,' '); break;
				case DOOR: 		 printCell(1,'c'); break;
				case ROOM: 		 printCell(4,' '); break;
				case CORRIDOR: printCell(2,'c'); break;
				case WALL: 		 printCell(3,'c'); break;
			}
		}

	}
	gotoEndGame();
}

void clearLog(int *line){

	int i, j;

	attron(COLOR_PAIR(5));

	for(i = 1 ; i <= LINES_LOGS - 2 ; i++){
		for(j = 1 ; j <= COLS_LOGS - 2 ; j++){
			move(i,j+COLS_GAME);
			printw(".");
		}
	}
	refresh();
	*line = 0;
	gotoEndGame();
}

void addLog(char * message, int * line){

	char truncatedMsg[COLS_LOGS];

	move((*line)+1, COLS_GAME+1);
	attron(COLOR_PAIR(1));
	refresh();

	// On tronque le message, pour pas dépasser la taille de la zone de log
	strncpy(truncatedMsg, message, COLS_LOGS-2);
	truncatedMsg[COLS_LOGS-2] = '\0';

	// On affiche le message
	printw(truncatedMsg);

	// Si on a plus de place pour clear la zone de logs
	if(*line >= LINES_LOGS - 3) clearLog(line);
	else (*line)++;
	gotoEndGame();
}
