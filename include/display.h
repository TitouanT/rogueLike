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

#include <ncurses.h>
#include <string.h>

#include "cell.h"
#include "mapConst.h"
#include "character.h"

/** Paire de couleur générale */
#define GENERAL_COLOR   1

/** Paire de couleur des couloirs */
#define CORRIDOR_COLOR  2

/** Paire de couleur des murs */
#define WALL_COLOR      3

/** Paire de couleur des pièces */
#define ROOM_COLOR      4

/** Paire de couleur du joueur */
#define PLAYER_COLOR    5

/** Paire de couleur des objets */
#define OBJECTS_COLOR   6

/** Paire de couleur d'une porte ouverte */
#define OPENED_DOOR     7

/** Paire de couleur du joueur dans un couloir */
#define PLAYER_C_COLOR  8

/** Paire de couleur de la barre verte */
#define BAR_GREEN       9

/** Paire de couleur de la barre rouge */
#define BAR_RED        10

/** Paire de couleur du titre */
#define COLOR_TITLE    11

/** Paire de couleur des kits de santé */
#define MED_KIT_COLOR  12


/** Hauteur de la fenêtre du jeu */
#define LINES_GAME  LINES + 2
/** Largeur de la fenêtre du jeu */
#define COLS_GAME   COLUMNS + 2

/** Hauteur de la fenêtre de logs */
#define LINES_LOGS LINES_GAME
/** Largeur de la fenêtre de logs */
#define COLS_LOGS   50

/** Hauteur de la fenêtre de statistiques */
#define LINES_STATS  6
/** Largeur de la fenêtre de statistiques */
#define COLS_STATS COLS_GAME + COLS_LOGS



/** Touche retour en arrière (celle de ncurses ne fonctionne pas) */
#define KEY_RETURN 263
#define KEY_RETURN_MAC 127



void displayObjectives(int *lineLog, WINDOW *win_logs);
void init_screen();
void startScreen(WINDOW *win);
void selectionScreen(WINDOW *win, t_cell map[LINES][COLUMNS], t_character *player);
WINDOW *createWindow(int startX, int startY, int width, int height, char * label);
void deleteWindow(WINDOW *window);
void displayFloor(t_cell map[LINES][COLUMNS], t_character player, WINDOW *win);
void addLog(char * message, int * line, WINDOW *win);
void clearArea(WINDOW *win, int startX, int startY, int width, int height);
void clearLog(int *line, WINDOW *win);
void displayPlayer(t_character player, t_cell mat[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line);
void displayStats(t_character player, WINDOW *win);
void setFloorCheat(t_cell map[LINES][COLUMNS]);
void displayEnd(t_character player, WINDOW *win);
void abortGame();
void printInventory(t_character player, WINDOW *win, int *lineLog);

int konami (int key);

#endif
