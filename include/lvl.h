/**
  * \file
  * \brief Generation des étages
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef LVL_H
#define LVL_H

#include "mapConst.h" // pour LINSE, COLUMNS, NB_LVL
#include "cell.h" // OK

/** Génère un étage du donjon de façons aléatoire */
void randomFloor (t_cell map[LINES][COLUMNS], int lvl);

/** fournit des information sur les niveau générés */
int queryLvlData (t_lvl tabLvl[NB_LVL]);

/** en cas de lecture du niveau, met les infos*/
void setLvlData (t_lvl tabLvl[NB_LVL]);

void initStatRoom (void);


#endif /* end of include guard: LVL_H */
