#include "display.h"

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_GREEN   "\e[32m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_BLUE    "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN    "\e[36m"
#define ANSI_COLOR_WHITE   "\e[37m"
#define ANSI_COLOR_RESET   "\e[0m"


void displayString(char * str, char * color){
	printf("%s%s%s", color, str, ANSI_COLOR_RESET);
}
void displayFloor (t_cell map[][COLUMNS]) {
	int i, j;
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			switch (map[i][j].type) {
				case EMPTY: displayString(" ", ANSI_COLOR_RESET); break;
				case DOOR: displayString("▒", ANSI_COLOR_RED); break;
				case ROOM: displayString("█", ANSI_COLOR_WHITE); break;
				case CORRIDOR: displayString("░", ANSI_COLOR_WHITE); break;
				case WALL: displayString("░", ANSI_COLOR_CYAN); break;
			}
		}
		printf("\n");
	}
	printf("\n");
}
