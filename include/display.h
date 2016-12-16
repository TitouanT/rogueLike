/**
	* \file
	* \brief Gestion de l'affichage
	* \author MOTTIER Emeric
	* \author PELLOIN Valentin
	* \author TEYSSIER Titouan
	* \date 24 novembre 2016
	* \version 1.0
	*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>   // OK

#include "cell.h"      // OK
#include "mapConst.h"  // OK
#include "character.h" // OK
#include "monstre.h"   // OK

void displayObjectives(int *lineLog, WINDOW *win_logs);
void init_screen();
void startScreen(WINDOW *win);
void selectionScreen(WINDOW *win, t_cell map[LINES][COLUMNS], t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonster);
WINDOW *createWindow(int startX, int startY, int width, int height, char * label);
void deleteWindow(WINDOW *window);
void displayFloor(t_cell map[LINES][COLUMNS], t_character player, WINDOW *win, int visibleByGhost[LINES][COLUMNS]);
void addLog(char * message, int * line, WINDOW *win);
// void clearArea(WINDOW *win, int startX, int startY, int width, int height); //utilisée seulement dans display.c
void clearLog(int *line, WINDOW *win);
void displayPlayer(t_character player, t_cell mat[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line);
void displayStats(t_character player, WINDOW *win);
void setFloorCheat(t_cell map[LINES][COLUMNS]);
void displayEnd(t_character player, WINDOW *win);
//void abortGame(); //utilisée seulement dans display.c
void printInventory(t_character player, WINDOW *win, int *lineLog);
void printLineCenter(char *msg, int widthScreen, int line, WINDOW *win);
void displayMonster (WINDOW * win, t_monster monsters[NB_MONSTER_MAX], t_cell map[LINES][COLUMNS], int nbMonster, int currentLvl, int visibleByGhost[LINES][COLUMNS]);

#endif
