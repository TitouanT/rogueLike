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
	err ("*** Debut create Monster ***");
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
	err(msg);
	for (i = 0; i < NB_LVL; i++) {
		nbMonsterLvl[i] = (float) lvlData[i].nbRoom * (float) nbMonster / (float) nbRoomGame;
		verif += nbMonsterLvl[i];
		sprintf(msg, "\t lvl 1: %d pieces donc %d monstres", lvlData[i].nbRoom, nbMonsterLvl[i]);
		err(msg);
	}

	while (verif < nbMonster) {
		verif++;
		nbMonsterLvl[NB_LVL - 1]++;
		err("+1 pour l'etage 5");
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
			if (!set) err("      /!\\ MONSTER WARNING  /!\\");
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
	err ("*** Fin create Monster ***");
}

/**
  * \brief fonction qui test si un
  * \fn int isThereAMonster (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl, int * indexMonster)
  * \param monsters tableau qui contiendra chaque monstre
  * \param nbMonsterAtEnd pointeur pour enregistrer le nombre de monstre
  */
int isThereAMonster (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl, int * indexMonster) {
	int i;
	for (i = 0; i < nbMonster; i++) {
		if (monsters[i].hp > 0 && monsters[i].lvl == lvl && monsters[i].line == line && monsters[i].col == col) {
			*indexMonster = i;
			return TRUE;
		}
	}
	return FALSE;
}

int isItWalkableForAMonster (t_cell cell) {
	switch (cell.type) {
		case ROOM: case CORRIDOR: return TRUE; break;
		case DOORWAY:
			if (cell.state == dCLOSE) return FALSE;
			else return TRUE;
			break;
		default: return FALSE;
	}
}

int canMove (t_monster monsters[NB_MONSTER_MAX], int nbMonster, int line, int col, int lvl) {
	int i;
	for (i = 0; i < nbMonster; i++) {
		if (monsters[i].hp > 0 && monsters[i].lvl == lvl && monsters[i].line == line && monsters[i].col == col) return FALSE;
	}
	return TRUE;
}

void playerAttackMonster (t_character player, t_monster monsters[NB_MONSTER_MAX], int indexMonster) {
	if (didItHappen(player.agility)) monsters[indexMonster].hp -= player.pw;
}


void monsterAttackPlayer (t_monster monster, t_character * player) {
	if (didItHappen(monster.agility)){
		player -> hp -= monster.pw;
		flash();
		// une attaque est faite !
	}
	// sinon le monstre rate son attaque;
}

void moveGhost(t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int iGhost, t_character *player) {

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
}

void setVisibleByGhost (t_monster monsters[NB_MONSTER_MAX], int visibleByGhost[LINES][COLUMNS], t_character player) {
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
}

void moveMonster (t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int nbMonster, t_character *player) {
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


}
