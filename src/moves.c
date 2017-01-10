/**
 * \file
 * \brief Fonctions de déplacement du joueur sur la carte
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.1
 * \date 25 novembre 2016
 */
#include <ncurses.h>   // OK

#include "moves.h"     // OK
#include "loadLvl.h"   // OK
#include "character.h" // OK
#include "cell.h"      // OK
#include "mapConst.h"  // OK
#include "tools.h"     // OK
#include "display.h"   // OK
#include <time.h>      // OK
#include <unistd.h>
#include <time.h>

void my_delay(int i)    /*Pause l'application pour i ms*/
{
	err("<my_delay>", +1);
	
	// clock_t start,end;
	// start=clock();
	// while(((end=clock())-start)<=((i*CLOCKS_PER_SEC)/1000));
	struct timespec delay = {i/1000, (long) (i%1000) * 1000000};
	nanosleep(&delay, NULL); 
	err("</my_delay>", -1);
}
/**
	* \brief Teste si le joueur peut se déplacer (si il a suffisament de nourriture)
	* Le joueur a 25% de chance de pouvoir se déplacer même en ayant plus de nourriture
	*	\fn int canPlayerMove(t_character *player)
	* \param player Joueur
	* \return TRUE si le joueur peut se déplacer
	*/
int canPlayerMove(t_character *player){
	err("<canPlayerMove>", +1);
	if(player->food == 0){
		err("</canPlayerMove>", -1);
		return didItHappen(25);
	}
	err("</canPlayerMove>", -1);
	return TRUE;
}

/**
	* \brief Augmente la faim du joueur de façon aléatoire
	* Le joueur a 30% de chance de perdre de la nourriture à chaque mouvement
	*	\fn void augmenterFaim(t_character *player)
	* \param player Joueur
	*/
void augmenterFaim(t_character *player){
	err("<augmenterFaim>", +1);
	if(canPlayerMove(player)){
		if(player->food > 0 && didItHappen(30)) (player->food)--;
	}
	err("</augmenterFaim>", -1);
}

/**
	* \brief Evanouissement du joueur
	* Selectionne un endroit aléatoire de la carte, et le dé-mémorise
	*	\fn void passOut(t_cell map[LINES][COLUMNS])
	* \param map Carte
	*/
void passOut(t_cell map[LINES][COLUMNS]){
	err("<passOut>", +1);

	int line, column, height, width, maxHeight, maxWidth;
	int i, j;

	int aMinLen = 10, aMaxHeight = 15, aMaxWidth = 20;

	line   = randab(1, LINES - aMinLen -1);
	column = randab(1, COLUMNS - aMinLen -1);

	maxHeight = min(LINES - line - 1, aMaxHeight);
	height    = randab(aMinLen, maxHeight +1);

	maxWidth = min(COLUMNS - column - 1, aMaxWidth);
	width    = randab(aMinLen, maxWidth +1);

	for(i = line ; i < maxHeight + line ; i++){
		for(j = column ; j < maxWidth + column ; j++){
			map[i][j].isDiscovered = FALSE;
		}
	}
	err("</passOut>", -1);

}

/**
	* \brief Retourne vrai si la cellule est une composante d'une pièce
	*	\fn int bIsPartOfRoom(t_cell cell)
	* \param cell Un point sur la carte
	*/
int bIsPartOfRoom(t_cell cell){
	//err("<bIsPartOfRoom/>", 0);
	return (cell.type == ROOM || cell.type == WALL || cell.type == DOORWAY);
}

/**
	* \brief Effet aléatoire dans piège sur le joueur
	*	\fn void fallTrap(t_cell map[LINES][COLUMNS], t_character *perso, WINDOW *win_logs, int *lineLog, t_dir direction, t_monster monsters[NB_MONSTER_MAX], int nbMonster, WINDOW *win_game, int visibleByGhost[LINES][COLUMNS])
	* \param map Carte
	* \param perso joueur
	* \param win_logs Fenêtre où afficher les objectifs
	* \param lineLog Numéro de ligne de log
	* \param direction Direction de mouvement du joueur
	* \param monsters L'ensemble des monstres
	* \param nbMonster Nombre total de monstres
	* \param win_game Fenêtre  où afficher le jeu
	* \param visibleByGhost Matrice pour savoir ce que les fantomes rendent visible
	*/
void fallTrap(t_cell map[LINES][COLUMNS], t_character *perso, WINDOW *win_logs, int *lineLog, t_dir direction, t_monster monsters[NB_MONSTER_MAX], int nbMonster, WINDOW *win_game, int visibleByGhost[LINES][COLUMNS]){

	err("<fallTrap>", +1);
	t_character player;
	int trapType, lostLvl, lostHp;

	if( perso->lvl !=0){
		trapType=randab(0, 3);
	}
	else{
		trapType=randab(1, 3);
	}
	switch(trapType){
		case 0 :
				lostHp  = randab(0, 5);
				(perso->hp) = (perso->hp) - lostHp;
				// lostLvl = randab((perso->lvl), (perso->lvl)+2) * (-1);
				// changeLvl(map, perso, lostLvl);
				lostLvl = randab(1, 3);
				if (perso->lvl == NB_LVL - 1) lostLvl = NB_LVL - 1;
				changeLvl(map, perso, -lostLvl);
				addLog("Vous êtes tombé dans un trou...", lineLog, win_logs);
			break;
		case 1 :
				lostHp = randab(0, 5);
				(perso->hp) = (perso->hp) - lostHp;
				addLog("Attention, un L1 vous a jeté une carte, vous      vous êtes écorché !", lineLog, win_logs);
			break;
		case 2 :
				do {
					setVisibleByGhost (monsters, visibleByGhost, *perso);
					clearLog(lineLog, win_logs);
					displayFloor(map, *perso, win_game, visibleByGhost);
					displayPlayer(*perso, map, win_game, win_logs, lineLog);
					displayMonster (win_game, monsters, map, nbMonster, (*perso).lvl, visibleByGhost);
					my_delay(50);
					addLog("Regardez où vous mettez vos pieds, la femme de    ménage a lustré le sol.", lineLog, win_logs);
				} while (move_perso(direction, map, perso, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost));
			break;
		default : break;
	}
	err("</fallTrap>", -1);
}

/**
	* \brief Renvoi les coordonnées de la pièce où le joueur se trouve
	*	\fn t_pos startRoom(t_cell map[LINES][COLUMNS], t_character player)
	* \param map Carte
	* \param player joueur
	*/
t_pos startRoom(t_cell map[LINES][COLUMNS], t_character player){
	err("<startRoom>", +1);

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
	err("</startRoom>", -1);

	return position;
}

/**
	* \brief Marque la pièce où se trouve le joueur comme découverte
	*	\fn void markDiscoverRoom(t_cell map[LINES][COLUMNS], t_character player)
	* \param map Carte
	* \param player joueur
	*/
void markDiscoverRoom(t_cell map[LINES][COLUMNS], t_character player){
	err("<markDiscoverRoom>", +1);

	t_pos start = startRoom(map, player);

	int i, j = start.column;

	if(map[player.line][player.column].type == ROOM){

		for(i = start.line ; bIsPartOfRoom(map[i][start.column]); i++){
			for(j = start.column ; bIsPartOfRoom(map[i][j]) ; j++){
				map[i][j].isDiscovered = TRUE;
				if(map[i][j].nbObject > 0 && map[i][j].obj[0].type != TRAP){
					map[i][j].obj[0].isDiscovered=TRUE;
				}
			}
		}
	}
	err("</markDiscoverRoom>", -1);
}

/**
	* \brief Marque les cellules autour du joueur comme découverte
	*	\fn void markDiscover(t_cell map[LINES][COLUMNS], t_character player)
	* \param map Carte
	* \param player joueur
	*/
void markDiscover(t_cell map[LINES][COLUMNS], t_character player) {
	err("<markDiscover>", +1);
	int line = player.line - 1;
	int col = player.column - 1;

	int i, j;
	if(map[player.line][player.column].nbObject > 0 /*&& map[player.line][player.column].obj[0].type == TRAP*/){
		map[player.line][player.column].obj[0].isDiscovered = TRUE;
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (line+i >= 0 && line+i < LINES && col+j >= 0 && col+j < COLUMNS) {
				map[line+i][col+j].isDiscovered = TRUE;
				if (map[i][j].nbObject > 0 && map[i][j].obj[0].type != TRAP)
					map[i][j].obj[0].isDiscovered=TRUE;
			}
		}
	}
	err("</markDiscover>", -1);
}

/**
	* \brief Déplace le joueur à un escalier montant ou descendant mis en paramètre
	*	\fn int move2spawn(t_cell map[LINES][COLUMNS], t_character *perso, int stair)
	* \param map Carte
	* \param perso joueur
	* \param stair Escalier
	*/
int move2spawn(t_cell map[LINES][COLUMNS], t_character *perso, int stair){
	err("<move2spawn>", +1);

	int i, j;

	for(i = 0 ; i < LINES ; i++){
		for(j = 0 ; j < COLUMNS ; j++){
			if(map[i][j].nbObject != 0 && map[i][j].obj[0].type == stair){
				perso->line = i;
				perso->column = j;
				markDiscoverRoom(map, *perso);
				perso->nbMove++;
				err("</move2spawn>", -1);
				return TRUE;
			}
		}
	}
	err("</move2spawn>", -1);
	return FALSE;
}

/**
	* \brief Vérifie si l'endroit choisi est accessible
	*	\fn int bIsWalkable(t_cell cell)
	* \param cell Un point sur la carte
	*/
int bIsWalkable(t_cell cell){
	err("<bIsWalkable>", +1);
	int rep = FALSE;

	if(cell.type == ROOM || cell.type == CORRIDOR) {
		rep = TRUE;
	}
	else if(cell.type == DOORWAY){
		if(cell.state == dOPEN || cell.state == dNONE){
			rep = TRUE;
		}
	}
	err("</bIsWalkable>", -1);
	return rep;
}

/**
	* \brief Déplace le joueur et les conséquences de ce mouvement (nourriture, piège)
	*	\fn int move_perso(t_dir direction, t_cell map[LINES][COLUMNS], t_character *perso, WINDOW *win_logs, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster, WINDOW *win_game, int visibleByGhost[LINES][COLUMNS])
	* \param direction Direction de mouvement du joueur
	* \param map Carte
	* \param perso joueur
	* \param win_logs Fenêtre où afficher les objectifs
	* \param lineLog Numéro de ligne de log
	* \param monsters L'ensemble des monstres
	* \param nbMonster Nombre total de monstres
	* \param win_game Fenêtre  où afficher le jeu
	* \param visibleByGhost Matrice pour savoir ce que les fantomes rendent visible
	*/
int move_perso(t_dir direction, t_cell map[LINES][COLUMNS], t_character *perso, WINDOW *win_logs, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster, WINDOW *win_game, int visibleByGhost[LINES][COLUMNS]) {
	err("<move_perso>", +1);
	int success = FALSE;
	int line   = perso->line;
	int column = perso->column;
	int indexMonster;

	if(!canPlayerMove(perso)) return FALSE;


	// On veut regarder si il est possible d'aller en haut
	if(direction == UP){
		if(line > 0 && bIsWalkable(map[line-1][column])){
			perso->line -= 1;
			success = TRUE;
		}
	}

	if(direction == DOWN){
		if(line+1 < LINES && bIsWalkable(map[line+1][column])){
			perso->line += 1;
			success = TRUE;
		}
	}

	if(direction == LEFT){
		if(column > 0 && bIsWalkable(map[line][column-1])){
			perso->column -= 1;
			success = TRUE;
		}
	}

	if(direction == RIGHT){
		if(column+1 < COLUMNS && bIsWalkable(map[line][column+1])){
			perso->column += 1;
			success = TRUE;
		}
	}

	if(direction == UP_LEFT){
		if(line > 0 && column > 0 && bIsWalkable(map[line - 1][column - 1])){
			perso->column -= 1;
			perso->line   -= 1;
			success = TRUE;
		}
	}

	if(direction == UP_RIGHT){
		if(line > 0 && column + 1 < COLUMNS  && bIsWalkable(map[line - 1][column + 1])){
			perso->column += 1;
			perso->line   -= 1;
			success = TRUE;
		}
	}

	if(direction == DOWN_LEFT){
		if(line+1 < LINES && column > 0 && bIsWalkable(map[line + 1][column - 1])){
			perso->column -= 1;
				perso->line   += 1;
				success = TRUE;
		}
	}

	if(direction == DOWN_RIGHT){
		if(line+1 < LINES && column + 1 < COLUMNS  && bIsWalkable(map[line + 1][column + 1])){
			perso->column += 1;
			perso->line   += 1;
			success = TRUE;
		}
	}
	// On effectue certaines actions si le joueur a réussi à se deplacer
	if(success == TRUE){

		if (isThereAMonster (monsters, nbMonster, perso -> line, perso -> column, perso -> lvl, &indexMonster) == TRUE) {
			playerAttackMonster (*perso, monsters, indexMonster);
			perso -> line = line;
			perso -> column = column;
		}
		else markDiscover(map, *perso);


		perso->nbMove++;
		moveMonster(map, monsters, nbMonster, perso);

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
			if(didItHappen(25)) passOut(map);
		}

		/// Perte de vie si le joueur est malade.
		if(perso->isSick){
			(perso->hp)--;
			// Probabilité de ne plus être malade
			if(didItHappen(35)) perso->isSick = FALSE;
		}

		if(map[perso->line][perso->column].nbObject > 0 && map[perso->line][perso->column].obj[0].type==TRAP){
			fallTrap(map,perso, win_logs, lineLog, direction, monsters, nbMonster, win_game, visibleByGhost);
			err("tombé dans un piege !!", 0);
		}
	}

	err("</move_perso>", -1);
	return success;
}
