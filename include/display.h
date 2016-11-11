#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <panel.h>
#include "cell.h"
#include "mapConst.h"

#define LINES_GAME  22
#define COLS_GAME   92
#define LINES_LOGS LINES_GAME
#define COLS_LOGS   50
#define LINES_STATS  5
#define COLS_STATS COLS_GAME + COLS_LOGS


void displayFloor(t_cell map[][COLUMNS]);

#endif /* end of include guard: DISPLAY_H */
