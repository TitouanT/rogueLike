/**
  * \file
  * \brief fonctions de génération d'étage
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#include <stdlib.h>   // OK

#include "monstre.h"  // OK
#include "tools.h"    // OK
#include "lvl.h"      // OK
#include "cell.h"     // OK
#include "mapConst.h" // OK
#include "filePos.h"  // OK



/**
  * \brief crée les monstres pour la partie
  * \fn void createMonster (t_monster monsters[NB_MONSTER_MAX], int * nbMonsterAtEnd)
  * \param monsters tableau qui contiendra chaque monstre
  * \param nbMonsterAtEnd pointeur pour enregistrer le nombre de monstre
  */
void createMonster (t_monster monsters[NB_MONSTER_MAX], int * nbMonsterAtEnd) {
	err("<createMonster>", +1);
	char msg[100];
	int i, j, k, nbRoomGame = 0, nbMonsterLvl[NB_LVL], verif = 0;
	int iRoom, iLvl;
	int choice, set;
	int acc = 2;

	t_lvl lvlData[NB_LVL];

	int chanceMonsterType[GHOST];

	int nbMonster = randab(NB_MONSTER_MIN, NB_MONSTER_MAX + 1);
	*nbMonsterAtEnd = nbMonster;

	monsters[0] = (t_monster) {GHOST, "Claude", -1, -1, randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 50, 100, 0, -1, -1};
	monsters[1] = (t_monster) {GHOST, "Chappe", -1, -1, randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 50, 100, 1, -1, -1};

	// determination du nombre de monstre par étage
	queryLvlData(lvlData);
	for (i = 0; i < NB_LVL; i++) nbRoomGame += lvlData[i].nbRoom;
	sprintf(msg, "il y a %d piece dans le bat et %d monstres:", nbRoomGame, nbMonster);
	err(msg, 0);
	for (i = 0; i < NB_LVL; i++) {
		nbMonsterLvl[i] = (float) lvlData[i].nbRoom * (float) nbMonster / (float) nbRoomGame;
		verif += nbMonsterLvl[i];
		sprintf(msg, "\t lvl 1: %d pieces donc %d monstres", lvlData[i].nbRoom, nbMonsterLvl[i]);
		err(msg, 0);
	}

	while (verif < nbMonster) {
		verif++;
		nbMonsterLvl[NB_LVL - 1]++;
		err("+1 pour l'etage 5", 0);
	}

	// determination du type;
	for (i = 0; i < NB_LVL; i++) {
		switch (i) {
			case 0:
				chanceMonsterType[L1]     = 100;
				chanceMonsterType[L2]     = 0;
				chanceMonsterType[L3]     = 0;
				chanceMonsterType[MASTER] = 0;
				chanceMonsterType[DOC]    = 0;
				break;

			case 1:
				chanceMonsterType[L1]     = 20;
				chanceMonsterType[L2]     = 100;
				chanceMonsterType[L3]     = 0;
				chanceMonsterType[MASTER] = 0;
				chanceMonsterType[DOC]    = 0;
				break;

			case 2:
				chanceMonsterType[L1]     = 20;
				chanceMonsterType[L2]     = 40;
				chanceMonsterType[L3]     = 100;
				chanceMonsterType[MASTER] = 0;
				chanceMonsterType[DOC]    = 0;
				break;

			case 3:
				chanceMonsterType[L1]     = 0;
				chanceMonsterType[L2]     = 20;
				chanceMonsterType[L3]     = 40;
				chanceMonsterType[MASTER] = 100;
				chanceMonsterType[DOC]    = 0;
				break;

			case 4:
				chanceMonsterType[L1]     = 0;
				chanceMonsterType[L2]     = 0;
				chanceMonsterType[L3]     = 20;
				chanceMonsterType[MASTER] = 40;
				chanceMonsterType[DOC]    = 100;
				break;

			case 5:
				chanceMonsterType[L1]     = 0;
				chanceMonsterType[L2]     = 0;
				chanceMonsterType[L3]     = 0;
				chanceMonsterType[MASTER] = 50;
				chanceMonsterType[DOC]    = 100;
				break;

			defaulf:
				chanceMonsterType[L1]     = 20;
				chanceMonsterType[L2]     = 40;
				chanceMonsterType[L3]     = 60;
				chanceMonsterType[MASTER] = 80;
				chanceMonsterType[DOC]    = 100;
				break;
		}
		for (j = 0; j < nbMonsterLvl[i]; j++, acc++) {
			choice = rand()%100;
			set = FALSE;
			for (k = 0; k < GHOST && set == FALSE; k++) {
				if (choice < chanceMonsterType[k]) {
					monsters[acc].type = (t_type_monster) k;
					monsters[acc].lvl = i;
					set = TRUE;
				}
			}
			if (!set) err("      /!\\ MONSTER WARNING  /!\\", 0);
		}
	}

	// parametres en fonction du type
	for (i = 2; i < nbMonster; i++) {
		switch (monsters[i].type) {
			case L1:
				monsters[i].hp = 5;
				monsters[i].pw = 1;
				monsters[i].speed = 1;
				monsters[i].sight = 5;
				monsters[i].agility = 50;
				break;

			case L2:
				monsters[i].hp = 10;
				monsters[i].pw = 2;
				monsters[i].speed = 1;
				monsters[i].sight = 10;
				monsters[i].agility = 60;
				break;

			case L3:
				monsters[i].hp = 15;
				monsters[i].pw = 2;
				monsters[i].speed = 1;
				monsters[i].sight = 12;
				monsters[i].agility = 70;
				break;

			case MASTER:
				monsters[i].hp = 15;
				monsters[i].pw = 4;
				monsters[i].speed = 1;
				monsters[i].sight = 18;
				monsters[i].agility = 80;
				break;

			case DOC:
				monsters[i].hp = 20;
				monsters[i].pw = 4;
				monsters[i].speed = 1;
				monsters[i].sight = 20;
				monsters[i].agility = 90;
				break;

			default: break;
		}
	}

	// positionnement des monstres sur leur étage
	for (i = 0; i < nbMonster; i++) {
		iLvl = monsters[i].lvl;
		iRoom = rand() % lvlData[ iLvl ].nbRoom;

		monsters[i].line = randab(1, lvlData[ iLvl ].rooms[iRoom].height - 1) + lvlData[ iLvl ].rooms[iRoom].line;

		monsters[i].col = randab(1, lvlData[ iLvl ].rooms[iRoom].width - 1) + lvlData[ iLvl ].rooms[iRoom].column;
	}
	err("</createMonster>", -1);
}

/**
  * \brief fonction qui test si un monstre est present
  * \fn int isThereAMonster (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl, int * indexMonster)
  * \param monsters tableau qui contiendra chaque monstre
  * \param nbMonster pointeur pour enregistrer le nombre de monstre
  * \param line ligne que l'on test
  * \param col colonne que l'on test
  * \param lvl étage que l'on test
  * \param indexMonster variable qui contiendra la position du monstre dans le tableau si il est trouvé
  * \return TRUE si un monstre est présent
  * \return FALSE sinon
  */
int isThereAMonster (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl, int * indexMonster) {
	err("<isThereAMonster>", +1);
	int i;
	for (i = 0; i < nbMonster; i++) {
		if (monsters[i].hp > 0 && monsters[i].lvl == lvl && monsters[i].line == line && monsters[i].col == col) {
			*indexMonster = i;
			err("</isThereAMonster>", -1);
			return TRUE;
		}
	}
	err("</isThereAMonster>", -1);
	return FALSE;
}

/**
  * \brief test si un monstre peu se marcher sur une case de la map
  * \fn int isItWalkableForAMonster (t_cell cell)
  * \param cell la case à tester
  * \return TRUE si la case est praticable pour un monstre
  * \return FALSE sinon
  */
int isItWalkableForAMonster (t_cell cell) {
	//err("<isItWalkableForAMonster>", +1);
	int rep;
	switch (cell.type) {
		case ROOM: case CORRIDOR: rep = TRUE; break;
		case DOORWAY:
			if (cell.state == dCLOSE) rep = FALSE;
			else rep = TRUE;
			break;
		default: rep = FALSE;
	}
	//err("</isItWalkableForAMonster>", -1);
	return rep;
}

/**
  * \brief test si un monstre peu se déplacer sur une case de la map
  * \fn int canMove (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl)
  * \param monsters tableau qui contiendra chaque monstre
  * \param nbMonster taille du tableau monsters
  * \param line ligne que l'on test
  * \param col colonne que l'on test
  * \param lvl étage que l'on test
  * \return TRUE si un monstre est présent
  * \return FALSE sinon
  */
int canMove (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl) {
	err("<canMove>", +1);
	int tmp, rep;
	rep = isThereAMonster(monsters, nbMonster, line, col, lvl, &tmp);
	err("</canMove>", -1);
	return !rep;
}

/**
  * \brief attaque d'un monstre par le joueur
  * \fn void playerAttackMonster (t_character player, t_monster monsters[NB_MONSTER_MAX], int indexMonster)
  * \param monsters tableau qui contiendra chaque monstre
  * \param player personnage controllé par le joueur
  * \param indexMonster index du monstre que le joueur veut attaquer
  */
void playerAttackMonster (t_character player, t_monster monsters[NB_MONSTER_MAX], int indexMonster) {
	err("<playerAttackMonster>", +1);
	if (didItHappen(player.agility)) monsters[indexMonster].hp -= player.pw;
	err("</playerAttackMonster>", -1);
}

/**
  * \brief attaque du joueur par un monstre
  * \fn void monsterAttackPlayer (t_monster monster, t_character * player)
  * \param monster monstre qui attque
  * \param player personnage controllé par le joueur
  */
void monsterAttackPlayer (t_monster monster, t_character * player) {
	err("<monsterAttackPlayer>", +1);
	if (didItHappen(monster.agility)){
		player -> hp -= monster.pw;
		flash();
		// une attaque est faite !
	}
	err("</monsterAttackPlayer>", -1);
	// sinon le monstre rate son attaque;
}

/**
  * \brief déplacement des fantomes
  * \fn void moveGhost(t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int iGhost, t_character *player)
  * \param map carte de l'étage
  * \param monsters tableau qui contient tous es monstres
  * \param iGhost index du fantome dans monsters
  * \param player personnage controllé par le joueur
  */
void moveGhost(t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int iGhost, t_character *player) {

	err("<moveGhost>", +1);
	int line, col, hDir = 0, vDir = 0;
	if (monsters[iGhost].data2 == -1 || monsters[iGhost].data3 == -1) {
		monsters[iGhost].data2 = randab(0, LINES);
		monsters[iGhost].data3 = randab(0, COLUMNS);
	}
	line = monsters[iGhost].data2;
	col  = monsters[iGhost].data3;

	if (line > monsters[iGhost].line) vDir = 1;
	else if (line < monsters[iGhost].line) vDir = -1;

	if (col > monsters[iGhost].col) hDir = 1;
	else if (col < monsters[iGhost].col) hDir = -1;

	if (hDir == 0 && vDir == 0) monsters[iGhost].data2 = -1;

	monsters[iGhost].line += vDir;
	monsters[iGhost].col  += hDir;
	err("</moveGhost>", -1);
}

/**
  * \brief fonction qui indique ce qui rend visible un cercle de case autour du monstre
  * \fn void setVisibleByGhost (t_monster monsters[NB_MONSTER_MAX], int visibleByGhost[LINES][COLUMNS], t_character player)
  * \param monsters tableau qui contient tous es monstres
  * \param visibleByGhost matrice qui contient un 1 si la case est visible, 0 sinon.
  * \param player personnage controllé par le joueur
  */
void setVisibleByGhost (t_monster monsters[NB_MONSTER_MAX], int visibleByGhost[LINES][COLUMNS], t_character player) {
	err("<setVisibleByGhost>", +1);
	int radius = 10, centerLine, centerCol, nbL, nbC, line, col, iGhost, i, j;
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) visibleByGhost[i][j] = 0;
	}
	for (iGhost = 0; iGhost < 2; iGhost++) {
		if (monsters[iGhost].lvl == player.lvl && monsters[iGhost].hp > 0) {
			centerLine = monsters[iGhost].line;
			centerCol = monsters[iGhost].col;
			for (i = 0; i < radius * 2; i++) {
				for (j = 0; j < radius * 2; j++) {
					line = centerLine - radius + i;
					col = centerCol - radius + j;
					nbL = centerLine - line;
					nbC = centerCol - col;
					if (nbL * nbL + nbC * nbC < radius * radius && line >= 0 && line < LINES && col >= 0 && col < COLUMNS)
					visibleByGhost[line][col] = 1;
				}
			}
		}
	}
	err("</setVisibleByGhost>", -1);
}

/**
  * \brief déplacement de tous les monstres
  * \fn void moveMonster (t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int nbMonster, t_character *player)
  * \param map carte de l'étage
  * \param monsters tableau qui contient tous es monstres
  * \param nbMonster nombre de monstres dans monsters
  * \param player personnage controllé par le joueur
  */
void moveMonster (t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int nbMonster, t_character *player) {
	err("<moveMonster>", +1);
	if (monsters[0].lvl == player -> lvl && monsters[0].hp > 0)
		moveGhost(map, monsters, 0, player);

	if (monsters[1].lvl == player -> lvl && monsters[0].hp > 0)
		moveGhost(map, monsters, 1, player);

	int i, j, path[LINES][COLUMNS];
	int l, c, val;
	t_pos head;

	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) path[i][j] = -1;
	}

	head.line = player -> line;
	head.column = player -> column;

	path[head.line][head.column] = 0;
	file_init();
	do {
		l = head.line;
		c = head.column;
		val = path[l][c];
		if ( l+1 < LINES && path[l + 1][c] == -1 && isItWalkableForAMonster(map[l + 1][c])) {
			path[l + 1][c] = val + 1;
			head.line = l + 1;
			file_ajouter (head);
		}
		if ( l-1 >= 0 && path[l - 1][c] == -1 && isItWalkableForAMonster(map[l - 1][c])) {
			path[l - 1][c] = val + 1;
			head.line = l - 1;
			file_ajouter (head);
		}
		if ( c+1 < COLUMNS && path[l][c + 1] == -1 && isItWalkableForAMonster(map[l][c + 1])) {
			path[l][c + 1] = val + 1;
			head.column = c + 1;
			head.line = l;
			file_ajouter (head);
		}
		if ( c-1 >= 0 && path[l][c - 1] == -1 && isItWalkableForAMonster(map[l][c - 1])) {
			path[l][c - 1] = val + 1;
			head.column = c - 1;
			head.line = l;
			file_ajouter (head);
		}
		file_retirer(&head);
	} while (!file_est_vide());
	file_supprimer();

	for (i = 2; i < nbMonster; i++) {
		if (monsters[i].lvl == player -> lvl  && monsters[i].hp > 0) {
			l = monsters[i].line;
			c = monsters[i].col;
			val = path[l][c];
			if (val > 0) {
				if (val <= monsters[i].sight && val != 1) {

					if (l + 1 < LINES   && path[l + 1][c] == val - 1 && canMove(monsters, nbMonster, l + 1, c, player -> lvl)) {
						monsters[i].line = l + 1;
						val--;
					}

					else if (l - 1 >= 0 && path[l - 1][c] == val - 1 && canMove(monsters, nbMonster, l - 1, c, player -> lvl)) {
						monsters[i].line = l - 1;
						val--;
					}

					else if (c + 1 < COLUMNS && path[l][c + 1] == val - 1 && canMove(monsters, nbMonster, l, c + 1, player -> lvl)) {
						monsters[i].col  = c + 1;
						val--;
					}

					else if (c - 1 >= 0 && path[l][c - 1] == val - 1 && canMove(monsters, nbMonster, l, c - 1, player -> lvl)) {
						monsters[i].col  = c - 1;
						val--;
					}
				}
				if (val == 1) monsterAttackPlayer (monsters[i], player);
			}
		}
	}
	err("</moveMonster>", -1);

}
