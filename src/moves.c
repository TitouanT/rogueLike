#include <ncurses.h>
#include "global.h"

int bIsWalkable(t_cell cell){

  if(cell.type == ROOM || cell.type == CORRIDOR || (cell.type == DOORWAY && (cell.state == dOPEN || cell.state == dNONE)) {
    return TRUE;
  }

  return FALSE;


}

int bCanMove(t_dir direction, t_cell mat[LINES][COLUMNS], t_character perso){

  int line   = perso.line;
  int column = perso.column;

  // On veut regarder si il est possible d'aller en haut
  if(direction == UP){
    if(line > 0 && bIsWalkable(mat[line+1][column])){
      return TRUE;
    }
  }

  if(direction == DOWN){

  }



}
