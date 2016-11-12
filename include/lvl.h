#ifndef LVL_H
#define LVL_H

#include "cell.h"
#include "mapConst.h"

void randomFloor (t_cell map[LINES][COLUMNS], int step);
int readLvl (char * fileName, t_cell map[][COLUMNS]);
void writeLvl (char * fileName, t_cell map[][COLUMNS]);

/*

on a besoin de :
	createWorld
	loadNextlvl
	loadPrevlvl













*/
//void displayFloor (t_cell map[][COLUMNS]);

#endif /* end of include guard: LVL_H */
