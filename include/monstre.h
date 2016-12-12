/**
  * \file
  * \brief Définition des monstres
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef MONSTER_H
#define MONSTER_H

#include "cell.h" // OK
#include "mapConst.h" // OK
#include "character.h" // OK

/** Nombre maximal de monstre dans une partie */
#define NB_MONSTER_MAX 100

/** Nombre minimal de monstre dans une partie */
#define NB_MONSTER_MIN 50




/**
  * \enum t_type_monster
  * \brief liste les différents types de monstres présent sur la map
  */
typedef enum {
	L1,     /*<! Les L1 qui sont les mins forts ;p*/
	L2,     /*<! Les L2 de loin les meilleurs */
	L3,     /*<! Les L3 presque aussi bon que les L2*/
	MASTER, /*<! Les Masters, discret mais peuvent devenir très violent si on les taquine trop*/
	DOC,    /*<! Les Doctorants, très discret, fuire à vue*/
	GHOST   /*<! Les Fantomes, parfois utile parfois moins, tres dangereux car on ne peut pas les tuer*/
} t_type_monster;


/**
  * \struct t_monster
  * \brief contient les informations nécessaire à la manipulation de monstre
  */
typedef struct {
	t_type_monster type; /*<! */
	char name[100];      /*<! */

	int line;            /*<! la ligne sur laquelle il se tient ou qu'il survole*/
	int col;             /*<! la colonne qu'il hante*/
	int lvl;             /*<! l'étage ou il habite*/

	int hp;              /*<! ses points de vie*/
	int pw;              /*<! ses points d'attaque*/
	int speed;           /*<! sa vitesse de déplacement*/
	int sight;
	int agility;

	int data1;           /*<! .*/
	int data2;           /*<! ..*/
	int data3;           /*<! ...*/
} t_monster;

void createMonster (t_monster monsters[NB_MONSTER_MAX], int * nbMonster);
void moveMonster (t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int nbMonster, t_character * player);
void setVisibleByGhost (t_monster monsters[NB_MONSTER_MAX], int visibleByGhost[LINES][COLUMNS], t_character player);
int isThereAMonster (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int column, int lvl, int * indexMonster);
void playerAttackMonster (t_character player, t_monster monsters[NB_MONSTER_MAX], int indexMonster);

#endif /* end of include guard: MONSTER_H */
