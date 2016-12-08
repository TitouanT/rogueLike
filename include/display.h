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

#include <ncurses.h>   // pour le type WINDOW


#include "cell.h"      // pour la map
#include "mapConst.h"  // pour les dimensions de la map
#include "character.h" // pour l'affichage du player
#include "monstre.h"   // pour l'affichage des monstres

void displayObjectives(int *lineLog, WINDOW *win_logs);
void init_screen();
void startScreen(WINDOW *win);
void selectionScreen(WINDOW *win, t_cell map[LINES][COLUMNS], t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonsters);
WINDOW *createWindow(int startX, int startY, int width, int height, char * label);
void deleteWindow(WINDOW *window);
void displayFloor(t_cell map[LINES][COLUMNS], t_character player, WINDOW *win);
void addLog(char * message, int * line, WINDOW *win);
// void clearArea(WINDOW *win, int startX, int startY, int width, int height); //utilisée seulement dans display.c
void clearLog(int *line, WINDOW *win);
void displayPlayer(t_character player, t_cell mat[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line);
void displayStats(t_character player, WINDOW *win);
void setFloorCheat(t_cell map[LINES][COLUMNS]);
void displayEnd(t_character player, WINDOW *win);
//void abortGame(); //utilisée seulement dans display.c
void printInventory(t_character player, WINDOW *win, int *lineLog);
void displayMonster (WINDOW * win, t_monster monsters[NB_MONSTER_MAX], t_cell map[LINES][COLUMNS], int nbMonsters, int currentLvl);

#endif
