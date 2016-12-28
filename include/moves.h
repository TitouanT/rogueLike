/**
  * \file
  * \brief Fonctions de déplacement du joueur
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 05 decembre 2016
  * \version 2.71828182846
  */


#ifndef MOVES_H
#define MOVES_H

#include <ncurses.h>   // OK
#include "cell.h"      // OK
#include "mapConst.h"  // OK
#include "character.h" // OK
#include "monstre.h"   // OK

typedef enum {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT} t_dir;

void markDiscoverRoom(t_cell map[LINES][COLUMNS], t_character player);
int move2spawn(t_cell map[LINES][COLUMNS], t_character *perso, int stair);
int move_perso(t_dir direction, t_cell map[LINES][COLUMNS], t_character *perso, WINDOW *win_logs, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster, WINDOW *win_game, int visibleByGhost[LINES][COLUMNS]);
void markDiscover(t_cell map[LINES][COLUMNS], t_character player);


#endif
