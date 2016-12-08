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

int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW *win_game, int *lineLog);

#endif
