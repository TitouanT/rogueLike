#include "global.h"

void InitGameMap(t_cell map[LINES][COLUMNS]){
  for(int i=0;i<NB_LVL;i++){
    randomFloor(map);
    writeLvl(map,i);
  }
  readLvl(map,0);
}

void DOWN_LVL(t_cell map[LINES][COLUMNS],t_character *player){
	writeLvl(map,(player->lvl));
        (player->lvl)-- ;
        readLvl(map,(player->lvl));
        move2spawn(map, player, STAIRS_UP);
}

void UP_LVL(t_cell map[LINES][COLUMNS],t_character *player){
 	writeLvl(map,(player->lvl));
        (player->lvl)++;
        readLvl(map,(player->lvl));
        move2spawn(map, player, STAIRS_DOWN);
}

