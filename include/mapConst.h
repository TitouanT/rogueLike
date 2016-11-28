#ifndef MAPCONST_H
#define MAPCONST_H

/** Si mit à TRUE, les étages sont entièrements découverts par défaut */
#define DEBUG FALSE

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

/** Nombres moyen de nourriture par pièce */
#define AV_NB_FOOD_ROOM 2

/** Chance de tomber malade lorsqu'il mange de la nourriture */
#define CHANCE_SICK 20

/** Nombres moyen de piège par pièce */
#define AV_NB_TRAP_ROOM 0.5

#endif
