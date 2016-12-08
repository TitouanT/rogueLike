#include "monstre.h"
#include "tools.h"
#include "lvl.h"
#include "cell.h"


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

	monsters[0] = (t_monster) {GHOST, "Claude", -1, -1, randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 0, 0, 0};
	monsters[1] = (t_monster) {GHOST, "Chappe", -1, -1, randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 1, 0, 0};

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
				break;

			case L2:
				monsters[i].hp = 10;
				monsters[i].pw = 2;
				monsters[i].speed = 1;
				break;

			case L3:
				monsters[i].hp = 15;
				monsters[i].pw = 2;
				monsters[i].speed = 1;
				break;

			case MASTER:
				monsters[i].hp = 15;
				monsters[i].pw = 4;
				monsters[i].speed = 1;
				break;

			case DOC:
				monsters[i].hp = 20;
				monsters[i].pw = 4;
				monsters[i].speed = 1;
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

void writeMonster (t_monster monsters[NB_MONSTER_MAX]) {

}

void readMonster (t_monster monsters[NB_MONSTER_MAX]) {

}

// void moveMonster (t_cell map[][COLUMNS], t_monster monsters[NB_MONSTER_MAX], int nbMonster, t_player player) {
//
// }
