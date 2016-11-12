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

	attron(COLOR_PAIR(1));

	// On découpe le message en sous messages pour rentrer dans la zone de logs
	while(strlen(message) > COLS_LOGS-1) {

		move((*line)+1, COLS_GAME+1);
  	printw("%.*s", COLS_LOGS-2, message);
		refresh();

		(*line)++;
    message+=COLS_LOGS-2;
  }
	move((*line)+1, COLS_GAME+1);
  printw("%s", message);
	refresh();



	// Si on a plus de place pour clear la zone de logs
	if(*line >= LINES_LOGS - 3) clearLog(line);
	else (*line)++;
	gotoEndGame();
}

// int addLog(char * message, int * line){
//
// 	attron(COLOR_PAIR(1));
//
// 	// On découpe le message en sous messages pour rentrer dans la zone de logs
// 	while(strlen(message) > COLS_LOGS-1 && *line < LINES_LOGS - 4) {
//
// 		move((*line)+1, COLS_GAME+1);
// 		refresh();
//   	printw("%.*s", COLS_LOGS-2, message);
// 		(*line)++;
//     message+=COLS_LOGS-2;
//   }
// 	if (*line >= LINES_LOGS - 4) {
// 		move((*line)+1, COLS_GAME+1);
// 		printw("--More--");
// 		refresh();
// 		return message;
// 		// scanf("%*c");
// 		// clearLog(line);
// 		// *line = 0;
// 		// addLog(message, line);
// 	}
// 	else {
// 		move((*line)+1, COLS_GAME+1);
// 		refresh();
//   	printw("%s", message);
// 		(*line)++;
// 	}
// 	gotoEndGame();
// 	return NULL;
// }
