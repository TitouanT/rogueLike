/**
  * \file
  * \brief definition des structure utilisée
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef CELL_H
#define CELL_H

#include "mapConst.h"

/**
 * \enum t_type
 * \brief définit les types de cellules
 */
typedef enum {
	EMPTY,     /*!< une cellules vides*/
	WALL,      /*!< un murs*/
	ROOM,      /*!< le sol d'une pièce*/
	CORRIDOR,  /*!< le sol d'un couloir*/
	DOORWAY    /*!< l'interface entre un coulir et une pièce*/
} t_type;

/**
 * \enum t_typeObject
 * \brief définit les types d'objet présent sur la map
 */
typedef enum {
	objNONE,     /*!< pas d'objet*/
	STAIRS_UP,   /*!< un escalier vers l'étage supérieur*/
	STAIRS_DOWN, /*!< un escalier vers l'étage inférieur*/
	FOOD,        /*!< de la nourriture*/
	TRAP         /*!< un piège*/
} t_typeObject;

/**
 * \struct t_object
 * \brief contient différentes information sur un objet
 */
typedef struct {
	t_typeObject type;  /*!< le type de l'objet*/
	int isDiscovered;   /*!< si il a été découvert ou non*/
} t_object;

/**
 * \enum t_state
 * \brief différent état pour une cellule
 */
typedef enum {
	DEFAULT_STATE,  /*!< pas d'état particulier*/
	dNONE,          /*!< pour le type DOORWAY : pas de porte*/
	dOPEN,          /*!<                        une porte ouverte*/
	dCLOSE,         /*!<                        une porte fermée*/
	LIGHT,          /*!< pour le type ROOM: la pièce est éclairer*/
	DARK            /*!<                    la pièce est particulièrement sombre*/
} t_state;

/**
 * \struct t_cell
 * \brief contient des information sur une cellule
 */
typedef struct {
	t_type type;              /*!< son type*/
	t_state state;            /*!< son état*/
	int isDiscovered;         /*!< un booléens pour savoir si elle à été découverte ou non*/
	int nbObject;             /*!< le nombre d'objet qu'elle contient*/
	t_object obj[OBJ_NB_MAX]; /*!< un tableau qui liste tout ces objets*/
} t_cell;

/**
 * \struct t_room
 * \brief contient les informatin nécessaire à la manipulation d'une pièce
 */
typedef struct {
	int line;   /*!< la ligne ou se trouve l'angle supérieur gauche de la piece*/
	int column; /*!< la colonne ou se trouve l'angle supérieur gauche de la piece*/
	int height; /*!< la hauteur de la pieces*/
	int width;  /*!< la largeur de la piece*/
} t_room;

/**
 * \struct t_lvl
 * \brief contient les information nécessaire à la manipulation d'un étage
 */
typedef struct {
	int nbRoom;                /*!< le nombre de pieces qu'il contient*/
	t_room rooms[ROOM_NB_MAX]; /*!< un tableau qui liste les pieces de l'étage*/
} t_lvl;

#endif /* end of include guard: CELL_H */
