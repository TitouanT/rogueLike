/**
  * \file
  * \brief Fonctions d'interaction avec l'utilisateur
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 05 decembre 2016
  * \version 1.2
  */

#ifndef INTERACTION_H
#define INTERACTION_H

#include <ncurses.h>   // OK
#include "cell.h"      // OK
#include "mapConst.h"  // OK
#include "character.h" // OK
#include "monstre.h"   // OK


int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW * win_game, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster, int *isPlayerInvicible, int visibleByGhost[LINES][COLUMNS]);
int jeu(t_cell map[LINES][COLUMNS], t_monster monsters[NB_MONSTER_MAX], t_character *player);

#endif
