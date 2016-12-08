/**
  * \file
  * \brief definition des structures utilisées
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
 * \brief définit les types de cellule
 */
typedef enum {
	EMPTY,     /**< une cellule vide*/
	WALL,      /**< un mur*/
	ROOM,      /**< le sol d'une pièce*/
	CORRIDOR,  /**< le sol d'un couloir*/
	DOORWAY    /**< l'interface entre un couloir et une pièce*/
} t_type;

/**
 * \enum t_typeObject
 * \brief définit les types d'objets présents sur la map
 */
typedef enum {
	objNONE,     /**<  pas d'objet*/
	STAIRS_UP,   /**<  un escalier vers l'étage supérieur*/
	STAIRS_DOWN, /**< un escalier vers l'étage inférieur*/
	FOOD,        /**< de la nourriture*/
	MED_KIT,     /**< des kits de santé*/
	TRAP         /**< un piège*/
} t_typeObject;

/**
 * \struct t_object
 * \brief contient différentes informations sur un objet
 */
typedef struct {
	t_typeObject type;  /**< le type de l'objet*/
	int isDiscovered;   /**< si il a été découvert ou non*/
} t_object;

/**
 * \enum t_state
 * \brief différents états pour une cellule
 */
typedef enum {
	DEFAULT_STATE,  /**< pas d'état particulier*/
	dNONE,          /**< pour le type DOORWAY : pas de porte*/
	dOPEN,          /**<                        une porte ouverte*/
	dCLOSE,         /**<                        une porte fermée*/
	LIGHT,          /**< pour le type ROOM: la pièce est éclairée*/
	DARK            /**<                    la pièce est particulièrement sombre*/
} t_state;

/**
 * \struct t_cell
 * \brief contient des informations sur une cellule
 */
typedef struct {
	t_type type;              /**< son type*/
	t_state state;            /**< son état*/
	int isDiscovered;         /**< un booléen pour savoir si elle à été découverte ou non*/
	int nbObject;             /**< le nombre d'objet qu'elle contient*/
	t_object obj[OBJ_NB_MAX]; /**< un tableau qui liste tout ses objets*/
} t_cell;

/**
 * \struct t_room
 * \brief contient les informations nécessaires à la manipulation d'une pièce
 */
typedef struct {
	int line;   /**< la ligne ou se trouve l'angle supérieur gauche de la piece*/
	int column; /**< la colonne ou se trouve l'angle supérieur gauche de la piece*/
	int height; /**< la hauteur de la piece*/
	int width;  /**< la largeur de la piece*/
} t_room;

/**
 * \struct t_lvl
 * \brief contient les informations nécessaire à la manipulation d'un étage
 */
typedef struct {
	int nbRoom;                         /**< le nombre de pieces qu'il contient*/
	t_room rooms[ROOM_NB_MAX + NB_LVL]; /**< un tableau qui liste les pieces de l'étage*/
} t_lvl;

/**
  * \struct t_pos
  * \brief décrit une position dans un étage
  */
typedef struct {
	int line;   /*!< représente la ligne */
	int column; /*!< représente la colonne */
} t_pos;


#endif /* end of include guard: CELL_H */
