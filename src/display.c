#include "display.h"

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_GREEN   "\e[32m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_BLUE    "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN    "\e[36m"
#define ANSI_COLOR_WHITE   "\e[37m"
#define ANSI_COLOR_RESET   "\e[0m"



void printCell(int pair, char cell){

	// Init pair : init_pair(ID_PAIR, TEXT COLOR, BACKGROUND COLOR);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);

	attron(COLOR_PAIR(pair));

	switch (cell) {
		case ' ': printw(" "); break;
		case 'c': addch(ACS_CKBOARD); break;
	}
}
void displayFloor (t_cell map[][COLUMNS]) {

	int i, j;

	for (i = 0; i < LINES; i++) {
		move(i+1,1);
		for (j = 0; j < COLUMNS; j++) {
			switch (map[i][j].type) {
				case EMPTY: 	 printCell(1,' '); break;
				case DOOR: 		 printCell(1,'c'); break; //changeColor("▒", ANSI_COLOR_RED); break;
				case ROOM: 		 printCell(4,' '); break; //changeColor("█", ANSI_COLOR_WHITE); break;
				case CORRIDOR: printCell(2,'c'); break; //changeColor("░", ANSI_COLOR_WHITE); break;
				case WALL: 		 printCell(3,'c'); break; //changeColor("░", ANSI_COLOR_CYAN); break;
			}
		}

	}
	move(LINES_STATS + LINES_GAME, 0);
}
