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
int canPlayerMove(t_character *player);
void augmenterFaim(t_character *player);
void cheat(WINDOW *win_logs, WINDOW *win_game, t_cell map[LINES][COLUMNS], t_character *player);
void passOut(t_cell map[LINES][COLUMNS]);
void fallTrap(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win_logs, int *lineLog, t_dir direction);
void grabItem(t_character *player, t_cell map[LINES][COLUMNS], WINDOW *win_logs, int *lineLog);
void dropItem(t_character *player, t_cell map[LINES][COLUMNS], WINDOW *win_logs, int *lineLog);

#endif
