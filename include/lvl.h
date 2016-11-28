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

#include "cell.h"

/** Génère un étage du donjon de façons aléatoire */
void randomFloor (t_cell map[LINES][COLUMNS], int lvl);

/** fournit des information sur les niveau générés */
void queryLvlData (t_lvl tabLvl[NB_LVL]);


#endif /* end of include guard: LVL_H */
