#ifndef MOVES_H
#define MOVES_H


typedef enum {UP, DOWN, LEFT, RIGHT} t_dir;

void markDiscoverRoom(t_cell map[LINES][COLUMNS], t_character player);
int move2spawn(t_cell mat[LINES][COLUMNS], t_character *perso);
int move_perso(t_dir direction, t_cell Mat[LINES][COLUMNS], t_character *perso);
void markDiscover(t_cell map[LINES][COLUMNS], t_character player);


#endif
