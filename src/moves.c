#include <ncurses.h>
#include "global.h"

int bIsWalkable(t_cell cell){

  if(cell == fxROOM || cell.f == fxCORRIDOR || cell == fxdOPEN){
    return TRUE;
  }

  return FALSE;


}

int bCanMove(t_dir direction, t_cell Mat[LINES][COLUMNS], t_pos position){

  int line   = position.line;
  int column = position.column;

  // On veut regarder si il est possible d'aller en haut
  if(direction == dirUP){
    if(line > 0 && bIsWalkable(Mat[line+1][column])){
      return TRUE;
    }
  }

  if(direction == dirDOWN){

  }



}
