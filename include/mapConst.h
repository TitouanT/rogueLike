/**
  * \file
  * \brief Définition des constantes qui paramètre le jeu
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef MAPCONST_H
#define MAPCONST_H
#include <ncurses.h> // OK

/** Si mit à TRUE, les étages sont entièrements découverts par défaut */
#define DEBUG FALSE

#define PRINT_ERROR TRUE

/** Nombre de ligne d'un étage */
#define LINES 36

/** Nombre de colonne d'un étage */
#define COLUMNS 118

/** Nombre d'étage dans une partie */
#define NB_LVL 6

/** Nombre minimal de pièce pour un étage */
#define ROOM_NB_MIN 5

/** Nombre maximal de pièce pour un étage */
#define ROOM_NB_MAX 10

/** Hauteur maximal d'une pièce */
#define ROOM_MAX_HEIGHT 10

/** Largeur maximal d'une pièce */
#define ROOM_MAX_WIDTH 20

/** Hauteur et Largeur minimal d'une pièce */
#define ROOM_MIN_LEN 5

/** Nombre d'objets maximal sur une cellule */
#define OBJ_NB_MAX 20

/** Nombre moyen de nourriture par pièce */
#define AV_NB_FOOD_ROOM 2

/** Nombre moyen de kits de santé par pièce */
#define AV_NB_MED_KIT_ROOM 0.5

/** Chance de tomber malade lorsqu'il mange de la nourriture */
#define CHANCE_SICK 20

/** Nombre moyen de piège par pièce */
#define AV_NB_TRAP_ROOM 0.5

/** Nombre d'items maximum dans l'inventaire du joueur */
#define SIZE_INVENTORY 5

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


#define CONTINUE_GAME 1

#define NEW_GAME 0

#endif
