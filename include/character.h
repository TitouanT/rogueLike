/**
  * \file
  * \brief définition du personnage
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_HP 10
#define MAX_FOOD 100


/**
 * \struct t_object
 * \brief contient les information nécessaire à la manipulation d'un personnage
 */
typedef struct {
	char name[100];  /*!< son nom*/
	int line;        /*!< la ligne a laquelle il se trouve*/
	int column;      /*!< la colonne à laquelle il se trouve*/
	int lvl;         /*!< l'étage auquel il se trouve*/
	int hp;          /*!< ces points de vie*/
	int pw;          /*!< ces points de dégats*/
	int xp;          /*!< son expérience*/
	int nbMove;      /*!< le nombre de mouvement effectué*/
	int food;        /*!< faible lorsque il a faim, haut lorsqu'il n'as pas faim*/
	int isSick;      /*!< un booléen pour savoir si il est malade*/
	int hasFoundObj; /*!< in booléen pour savoir si il a trouvé l'OBJET du jeu*/
} t_character;

#endif
