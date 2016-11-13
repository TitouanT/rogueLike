#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <panel.h>
#include <string.h>

#include "cell.h"
#include "mapConst.h"
#include "character.h"

#define COLOR_EMPTY COLOR_BLACK
#define COLOR_LIGHT COLOR_WHITE
#define COLOR_PLAYER_F COLOR_MAGENTA
#define COLOR_PLAYER_B COLOR_BLUE
#define COLOR_DOOR_OPEN COLOR_GREEN
#define COLOR_DOOR_CLOSE COLOR_RED
#define COLOR_WALL COLOR_CYAN
#define COLOR_FLOOR COLOR_YELLOW



// Taille de la fenetre du jeu
#define LINES_GAME  LINES + 2
#define COLS_GAME   COLUMNS + 2

// Taille de la fenetre de logs
#define LINES_LOGS LINES_GAME
#define COLS_LOGS   50

// Taille de la fenetre de statistiques
#define LINES_STATS  5
#define COLS_STATS COLS_GAME + COLS_LOGS


WINDOW *createWindow(int startX, int startY, int width, int height, char * label);

void displayFloor(t_cell map[LINES][COLUMNS], WINDOW *win);
void addLog(char * message, int * line, WINDOW *win);
void clearLog(int *line, WINDOW *win);
void displayPlayer(t_character player, t_cell mat[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line);
void displayStats(t_character player, WINDOW *win);

#endif /* end of include guard: DISPLAY_H */
