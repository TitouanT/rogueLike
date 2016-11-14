#include <ncurses.h>
#include "global.h"
#include "filePos.h"


t_pos startRoom(t_cell map[LINES][COLUMNS], t_character player){

	int line   = player.line;
	int column = player.column;

	t_pos position;

	int distYpos = 0, distYneg = 0;
	int distXpos = 0, distXneg = 0;

	int i;

	for(i = line ; map[i][column].type == ROOM ; i++) distYneg++;
	for(i = line ; map[i][column].type == ROOM ; i--) distYpos++;
	for(i = column ; map[line][i].type == ROOM ; i++) distXpos++;
	for(i = column ; map[line][i].type == ROOM ; i--) distXneg++;

	position.line = line - distYpos;
	position.column = column - distXneg;

	return position;
}

int bIsPartOfRoom(t_cell cell){

	return (cell.type == ROOM || cell.type == WALL || cell.type == DOORWAY);

}

void markDiscoverRoom(t_cell map[LINES][COLUMNS], t_character player){

	t_pos start = startRoom(map, player);

	int i, j = start.column;

	if(map[player.line][player.column].type == ROOM){

		for(i = start.line ; bIsPartOfRoom(map[i][start.column]); i++){
			for(j = start.column ; bIsPartOfRoom(map[i][j]) ; j++){
				map[i][j].isDiscovered = TRUE;
			}
		}
		
	}



}


void markDiscover(t_cell map[LINES][COLUMNS], t_character player) {
	int line = player.line - 1;
	int col = player.column - 1;
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (line+i >= 0 && line+i < LINES && col+j >= 0 && col+j < COLUMNS) map[line+i][col+j].isDiscovered = TRUE;
		}
	}
}


int move2spawn(t_cell mat[LINES][COLUMNS], t_character *perso){

  int i, j;

  for(i = 0 ; i < LINES ; i++){
    for(j = 0 ; j < COLUMNS ; j++){

      if(mat[i][j].nbObject != 0 && mat[i][j].obj[0] == STAIRS_DOWN){
        perso->line = i;
        perso->column = j;
				markDiscover(mat, *perso);
        return TRUE;
      }

    }
  }

  return FALSE;
}

int move2Stairs_UP(t_cell mat[LINES][COLUMNS], t_character *perso){

  int i, j;

  for(i = 0 ; i < LINES ; i++){
    for(j = 0 ; j < COLUMNS ; j++){

      if(mat[i][j].nbObject != 0 && mat[i][j].obj[0] == STAIRS_UP){
        perso->line = i;
        perso->column = j;
				markDiscover(mat, *perso);
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
