#include "cell.h"
#include "mapConst.h"
#include "monster.h"

int nbMonster;
t_monster monsters[NB_MONSTER_MAX];

void initMonster () {
	int i;
	nbMonster = randab(NB_MONSTER_MIN, NB_MONSTER_MAX + 1);
	monster[0] = {GHOST, "Claude", randab(0, LINES), randab(0, COLUMNS),
								randab(NB_LVL - 3, NB_LVL), 100, 1, 1, 0, 0, 0};
	for (i = 1; i < nbMonster; i++) {
		
	}
}
