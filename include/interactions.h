#ifndef INTERACTION_H
#define INTERACTION_H


int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW *win_game, int *lineLog);
int canPlayerMove(t_character *player);
void augmenterFaim(t_character *player);
void cheat(WINDOW *win_logs, WINDOW *win_game, t_cell map[LINES][COLUMNS], t_character *player);
void passOut(t_cell map[LINES][COLUMNS]);

#endif
