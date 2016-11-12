#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <panel.h>
#include <string.h>

#include "cell.h"
#include "mapConst.h"


// Taille de la fenetre du jeu
#define LINES_GAME  LINES + 2
#define COLS_GAME   COLUMNS + 2

// Taille de la fenetre de logs
#define LINES_LOGS LINES_GAME
#define COLS_LOGS   50

// Taille de la fenetre de statistiques
#define LINES_STATS  5
#define COLS_STATS COLS_GAME + COLS_LOGS



void displayFloor(t_cell map[][COLUMNS]);
void addLog(char * message, int * line);
void clearLog();

#endif /* end of include guard: DISPLAY_H */
