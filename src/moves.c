#include <ncurses.h>
#include "global.h"
#include "moves.h"


int move2spawn(t_cell mat[LINES][COLUMNS], t_character *perso){

  int i, j;

  for(i = 0 ; i < LINES ; i++){
    for(j = 0 ; j < COLUMNS ; j++){

      if(mat[i][j].nbObject != 0 && mat[i][j].obj[0] == STAIRS_DOWN){
        perso->line = i;
        perso->column = j;
				mat[i][j].isDiscovered = TRUE;
        return TRUE;
      }

    }
  }

  return FALSE;
}

int bIsWalkable(t_cell cell){

  if(cell.type == ROOM || cell.type == CORRIDOR) {
    return TRUE;
  }
  if(cell.type == DOORWAY){
    if(cell.state == dOPEN || cell.state == dNONE){
      return TRUE;
    }
  }

  return FALSE;

}

int move_perso(t_dir direction, t_cell mat[LINES][COLUMNS], t_character *perso){

  int line   = perso->line;
  int column = perso->column;

  // On veut regarder si il est possible d'aller en haut
  if(direction == UP){
    if(line > 0 && bIsWalkable(mat[line-1][column])){
      perso->line -= 1;
      return TRUE;
    }
  }

  if(direction == DOWN){
    if(line+1 < LINES && bIsWalkable(mat[line+1][column])){
      perso->line += 1;
      return TRUE;
    }
  }

  if(direction == LEFT){
    if(column > 0 && bIsWalkable(mat[line][column-1])){
      perso->column -= 1;
      return TRUE;
    }
  }

  if(direction == RIGHT){
    if(line+1 < COLUMNS && bIsWalkable(mat[line][column+1])){
      perso->column += 1;
      return TRUE;
    }
  }

  return FALSE;

}
