#ifndef INTERACTION_H
#define INTERACTION_H


int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, int *lineLog);
int canPlayerMove(t_character *player);
void augmenterFaim(t_character *player);

#endif
