#include "monstre.h"
#include "tools.h"
#include "lvl.h"
#include "t_cell.h"

int nbMonster;
t_monster monsters[NB_MONSTER_MAX];

void createMonster () {
	int i, nbRoomGame = 0, nbMonsterLvl[NB_LVL];
	float nbMonsterPerRoom;
	t_lvl lvlData[NB_LVL];
	nbMonster = randab(NB_MONSTER_MIN, NB_MONSTER_MAX + 1);
	monster[0] = {GHOST, "Claude", randab(0, LINES), randab(0, COLUMNS),
								randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 0, 0, 0};
	monster[1] = {GHOST, "Chappe", randab(0, LINES), randab(0, COLUMNS),
								randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 1, 0, 0};

	queryLvlData(lvlData);
	for (i = 0; i < NB_LVL) nbRoomGame += lvlData[i].nbRoom;

	nbMonsterPerRoom = (float) nbMonster / (float) nbRoomGame;


	for (i = 2; i < nbMonster; i++) {
		// mm c
		monster[i].lvl = didItHappen();
		monster[i].type = randab (L1, GHOST);
		monster[i].line = -1;
		monster[i].col  = -1;
		switch (monster[i].type) {
			case L1: {
				monster[i].lvl = didItHappen();
			} break;
			case L2:
			case L3:
			case MASTER:
			case DOC:
		}
	}
}
