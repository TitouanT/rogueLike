#include "display.h"

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_GREEN   "\e[32m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_BLUE    "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN    "\e[36m"
#define ANSI_COLOR_WHITE   "\e[37m"
#define ANSI_COLOR_RESET   "\e[0m"



void changeColor(char * color){
	//printf("%s%s%s", color, str, ANSI_COLOR_RESET);
	addch(ACS_CKBOARD);
}
void displayFloor (t_cell map[][COLUMNS]) {
	int i, j;


	start_color();
	// Init pair : init_pair(ID_PAIR, TEXT COLOR, BACKGROUND COLOR);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);



	for (i = 0; i < LINES; i++) {
		move(i+1,1);
		for (j = 0; j < COLUMNS; j++) {
			switch (map[i][j].type) {
				case EMPTY: 	 printw(" "); break;
				case DOOR: 		 attron(COLOR_PAIR (1)); addch(ACS_CKBOARD); break; //changeColor("▒", ANSI_COLOR_RED); break;
				case ROOM: 		 attron(COLOR_PAIR (4)); printw(" "); break; //changeColor("█", ANSI_COLOR_WHITE); break;
				case CORRIDOR: attron(COLOR_PAIR (2)); addch(ACS_CKBOARD); break; //changeColor("░", ANSI_COLOR_WHITE); break;
				case WALL: 		 attron(COLOR_PAIR (3)); addch(ACS_CKBOARD); break; //changeColor("░", ANSI_COLOR_CYAN); break;
			}
		}

	}
	move(LINES_STATS + LINES_GAME, 0);
}
