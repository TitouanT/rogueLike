#ifndef LOADLVL_H
#define LOADLVL_H

void InitGameMap(t_cell map[LINES][COLUMNS]);
void DOWN_LVL(t_cell map[LINES][COLUMNS],t_character *player);
void UP_LVL(t_cell map[LINES][COLUMNS],t_character *player);

#endif 
