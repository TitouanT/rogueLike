#ifndef LVL_H
#define LVL_H

#include "cell.h"
#include "mapConst.h"


void randomFloor (t_cell map[LINES][COLUMNS]);
int isBetween (int val, int a, int b);

/*

on a besoin de :
	createWorld ()
	loadNextlvl (map, lvlCourant)
	loadPrevlvl (map, lvlCourant)













*/
//void displayFloor (t_cell map[][COLUMNS]);

#endif /* end of include guard: LVL_H */
