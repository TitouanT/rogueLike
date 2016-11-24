#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <string.h>

#include "cell.h"
#include "mapConst.h"
#include "character.h"

#define DARK_GREY 8
#define LIGHT_GREEN 10

#define GENERAL_COLOR   1
#define CORRIDOR_COLOR  2
#define WALL_COLOR      3
#define ROOM_COLOR      4
#define PLAYER_COLOR    5
#define OBJECTS_COLOR   6
#define OPENED_DOOR     7
#define PLAYER_C_COLOR  8
#define BAR_GREEN       9
#define BAR_RED        10
#define COLOR_TITLE    11


// Taille de la fenetre du jeu
#define LINES_GAME  LINES + 2
#define COLS_GAME   COLUMNS + 2

// Taille de la fenetre de logs
#define LINES_LOGS LINES_GAME
#define COLS_LOGS   50

// Taille de la fenetre de statistiques
#define LINES_STATS  6
#define COLS_STATS COLS_GAME + COLS_LOGS

#define KEY_RETURN 127

void displayObjectives(int *lineLog, WINDOW *win_logs);
void init_screen();
void startScreen(WINDOW *win);
void selectionScreen(WINDOW *win);
WINDOW *createWindow(int startX, int startY, int width, int height, char * label);
void deleteWindow(WINDOW *window);
void displayFloor(t_cell map[LINES][COLUMNS], WINDOW *win);
void addLog(char * message, int * line, WINDOW *win);
void clearArea(WINDOW *win, int startX, int startY, int width, int height);
void clearLog(int *line, WINDOW *win);
void displayPlayer(t_character player, t_cell mat[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line);
void displayStats(t_character player, WINDOW *win);
void setFloorCheat(t_cell map[LINES][COLUMNS]);

#endif /* end of include guard: DISPLAY_H */
