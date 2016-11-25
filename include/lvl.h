#ifndef LVL_H
#define LVL_H

#include "cell.h"


void randomFloor (t_cell map[LINES][COLUMNS], int lvl);
void queryLvlData (t_lvl tabLvl[NB_LVL]);
/*

on a besoin de :
	createWorld ()
	loadNextlvl (map, lvlCourant)
	loadPrevlvl (map, lvlCourant)













*/
//void displayFloor (t_cell map[][COLUMNS]);

#endif /* end of include guard: LVL_H */
