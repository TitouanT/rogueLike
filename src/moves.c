/**
 * \file
 * \brief Fonctions de déplacement du joueur sur la carte
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.1
 * \date 25 novembre 2016
 */
#include <ncurses.h>
#include "global.h"
#include "filePos.h"

/* Renvoi les coordonnées de la pièce où le joueur se trouve */
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

/* Retourne vrai si la cellule est une composante d'une pièce */
int bIsPartOfRoom(t_cell cell){
	return (cell.type == ROOM || cell.type == WALL || cell.type == DOORWAY);
}

/* Marque la pièce où se trouve le joueur comme découverte */
void markDiscoverRoom(t_cell map[LINES][COLUMNS], t_character player){

	t_pos start = startRoom(map, player);

	int i, j = start.column;

	if(map[player.line][player.column].type == ROOM){

		for(i = start.line ; bIsPartOfRoom(map[i][start.column]); i++){
			for(j = start.column ; bIsPartOfRoom(map[i][j]) ; j++){
				map[i][j].isDiscovered = TRUE;
				if(map[i][j].obj[0].type!=TRAP){
					map[i][j].obj[0].isDiscovered=TRUE;
				}
			}
		}
	}
}

/* Marque les cellules autour du joueur comme découverte */
void markDiscover(t_cell map[LINES][COLUMNS], t_character player) {

	int line = player.line - 1;
	int col = player.column - 1;

	int i, j;
	if(map[player.line][player.column].obj[0].type==TRAP){
		map[player.line][player.column].obj[0].isDiscovered=TRUE;
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (line+i >= 0 && line+i < LINES && col+j >= 0 && col+j < COLUMNS) map[line+i][col+j].isDiscovered = TRUE; map[i][j].obj[0].isDiscovered=TRUE;
		}
	}
}

/* Déplace le joueur au spawn */
int move2spawn(t_cell mat[LINES][COLUMNS], t_character *perso, int stair){

  int i, j;

  for(i = 0 ; i < LINES ; i++){
    for(j = 0 ; j < COLUMNS ; j++){

      if(mat[i][j].nbObject != 0 && mat[i][j].obj[0].type == stair){
        perso->line = i;
        perso->column = j;
				markDiscoverRoom(mat, *perso);
				perso->nbMove++;
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
	err("***debut move perso***");
	int success = FALSE;
	int line   = perso->line;
	int column = perso->column;

	if(!canPlayerMove(perso)) return FALSE;


  // On veut regarder si il est possible d'aller en haut
  if(direction == UP){
    if(line > 0 && bIsWalkable(mat[line-1][column])){
      perso->line -= 1;
			success = TRUE;
    }
  }

  if(direction == DOWN){
    if(line+1 < LINES && bIsWalkable(mat[line+1][column])){
      perso->line += 1;
			success = TRUE;
    }
  }

  if(direction == LEFT){
    if(column > 0 && bIsWalkable(mat[line][column-1])){
      perso->column -= 1;
			success = TRUE;
    }
  }

  if(direction == RIGHT){
    if(column+1 < COLUMNS && bIsWalkable(mat[line][column+1])){
      perso->column += 1;
			success = TRUE;
    }
  }

	if(direction == UP_LEFT){
		if(line > 0 && column > 0 && bIsWalkable(mat[line - 1][column - 1])){
			perso->column -= 1;
			perso->line   -= 1;
			success = TRUE;
		}
	}

	if(direction == UP_RIGHT){
    if(line > 0 && column + 1 < COLUMNS  && bIsWalkable(mat[line - 1][column + 1])){
			perso->column += 1;
			perso->line   -= 1;
			success = TRUE;
    }
  }

	if(direction == DOWN_LEFT){
    if(line+1 < LINES && column > 0 && bIsWalkable(mat[line + 1][column - 1])){
			perso->column -= 1;
			perso->line   += 1;
			success = TRUE;
    }
  }

	if(direction == DOWN_RIGHT){
    if(line+1 < LINES && column + 1 < COLUMNS  && bIsWalkable(mat[line + 1][column + 1])){
			perso->column += 1;
			perso->line   += 1;
			success = TRUE;
    }
  }

	// On effectue certaines actions si le joueur a réussi à se deplacer
	if(success == TRUE){

		perso->nbMove++;

		augmenterFaim(perso);

		if(perso->food >= MAX_FOOD - 10 && perso->hp < MAX_HP && perso->isSick == FALSE){
			if(didItHappen(perso->food / 3)){
				(perso->hp)++;
				(perso->food) -= 10;
			}
		}
		else {
			if(didItHappen(1) && perso->hp < MAX_HP) (perso->hp)++;
		}

		/// Perte de vie si la personne a trop faim.
		if(perso->food <= 0){
			if(didItHappen(10)) (perso->hp)--;
		}
		if(perso->food <= 20){
			if(didItHappen(25)) passOut(mat);
		}

		/// Perte de vie si le joueur est malade.
		if(perso->isSick){
			(perso->hp)--;
			// Probabilité de ne plus être malade
			if(didItHappen(35)) perso->isSick = FALSE;

		}
		
		if(mat[perso->line][perso->column].obj[0].type==TRAP){
			fallTrap(mat,perso);
			err("***tombé dans un piege !!***");
			exit(1);
		}
	}

	err("***debut move perso***");
  	return success;

}
