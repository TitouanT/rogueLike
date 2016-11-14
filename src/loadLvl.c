#include "global.h"

void InitGameMap(	t_cell map[LINES][COLUMNS]){
  for(int i=0;i<NB_LVL;i++){
    randomFloor(map);
    writeLvl(map,i);
  }
  readLvl(map,0);
}
