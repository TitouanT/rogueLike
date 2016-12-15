/**
  * \file
  * \brief fonctions de génération d'étage
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#include <stdlib.h>   // OK

#include "cell.h"     // OK
#include "filePos.h"  // OK
#include "mapConst.h" // OK
#include "tools.h"    // OK




/** tableau qui contient tous les étages de la partie */
t_lvl gLvl[NB_LVL];

/** permet de savoir quel étage est actuellement en cours de génération */
int gLvlId = NB_LVL + 1;

/**
  * \brief initialise la gestion des informations sur les étages
  * \fn void initStatRoom (void)
  */
void initStatRoom () {
	gLvlId = 0;
}

/**
  * \brief place les informations sur les étages
  * \fn void setLvlData (t_lvl tabLvl[NB_LVL])
  * \param tabLvl tableau qui contient chaque niveau
  */
void setLvlData (t_lvl tabLvl[NB_LVL]) {
	err("*** debut set lvl data ***");
	int i, j;
	for (i = 0; i < NB_LVL; i++) {
		gLvl[i] = tabLvl[i];
	}
	err("\ncontenu tabLvl");
	gLvlId = NB_LVL;

	err("*** fin set lvl data ***");
}

/**
  * \brief donne les informations sur les étages
  * \fn int queryLvlData (t_lvl tabLvl[NB_LVL])
  * \param tabLvl tableau qui contiendra chaque niveau
  * \return TRUE si les infos sont présentent
  * \return FALSE sinon
  */
int queryLvlData (t_lvl tabLvl[NB_LVL]) {
	err("*** debut query lvl data ***");
	int i;
	if (gLvlId == NB_LVL) {
		for (i = 0; i < NB_LVL; i++) {
			tabLvl[i] = gLvl[i];
		}
		err("*** fin query lvl data ***");
		return TRUE;
	}
	else {
		err ("queryLvlData: impossible d'avoir toutes les informations sur les étage");
		err("*** fin query lvl data ***");
		return FALSE;
	}
}

/**
  * \brief initialise l'étage a créer
  * \fn void initFloor (t_cell map[LINES][COLUMNS])
  * \param map matrice qui représente le niveau
  */
void initFloor (t_cell map[LINES][COLUMNS]) {
	err("*** debut init floor ***");
	int i, j;
	for (i = 0; i < LINES; i++) for (j = 0; j < COLUMNS; j++) {
		map[i][j].type = EMPTY;
		map[i][j].state = DEFAULT_STATE;
		map[i][j].isDiscovered = DEBUG;
		map[i][j].nbObject = 0;

	}
	err("*** fin init floor ***");
}

/**
  * \brief fonction booléenne pour savoir si deux pieces se touchent
  * \fn int areInContact (t_room r1, t_room r2)
  * \param r1 premiere piece
  * \param r2 seconde piece
  * \return TRUE si les pieces se touchent
  * \return FALSE sinon.
  */
int areInContact (t_room r1, t_room r2) { // return TRUE if the room are in Contact
	if (intervalOverlaping(r2.line, r2.line + r2.height, r1.line, r1.line + r1.height)
	&& intervalOverlaping(r2.column, r2.column + r2.width, r1.column, r1.column + r1.width)) return TRUE;
	else return FALSE;
}

/**
  * \brief fonction qui renvoi une pièce valable
  * \fn t_room randomRoom (t_cell map[][COLUMNS], t_room * rooms, int nbRoom, int *nbTotal)
  * \param map carte représenant l'étage
  * \param rooms tableau qui contient toutes les pieces déjà créée
  * \param nbRoom nombre de pièces créée.
  * \param nbTotal nb de pieces a créé, sera mis à nbRoom en cas d'échec
  * \return room une piece valide
  */
t_room randomRoom (t_cell map[][COLUMNS], t_room * rooms, int nbRoom, int *nbTotal) {
	int maxHeight, maxWidth, i, j, posNotOk, maxTour = 100, acc;
	t_room room;
	acc = 0;
	do {
		// choose a tools position for the top left corner of the room
		room.line = randab(1, LINES - ROOM_MIN_LEN - 1);
		room.column = randab(1, COLUMNS - ROOM_MIN_LEN - 1);

		// the door dimensions are choose between the min length and the maximum they can be without going out of the map
		maxHeight = min(LINES - room.line - 1, ROOM_MAX_HEIGHT);
		room.height = randab(ROOM_MIN_LEN, maxHeight + 1);

		maxWidth = min(COLUMNS - room.column - 1, ROOM_MAX_WIDTH);
		room.width  = randab(ROOM_MIN_LEN, maxWidth + 1);

		// if the room is touching another one then try another
		posNotOk = 0;
		for (i = 0; i < nbRoom && posNotOk == 0; i++)
			if (areInContact(room, rooms[i])) posNotOk = 1;

		acc++;
	} while (posNotOk && acc < maxTour); // maxTour is there to protect against infinite loop

	if (acc >= maxTour) { //if acc reach maxTour then give up
		*nbTotal = nbRoom;
		room.line = 42;
		room.column = 42;
		room.height = 42;
		room.width = 42;
		return room;
	}

	// are you really reading that ?
	for (i = 0; i < room.height; i++) for (j = 0; j < room.width; j++) {

		if (i == 0 || i == room.height - 1 || j == 0 || j == room.width - 1) {
			map[room.line + i][room.column + j].type = WALL;
		}
		else {
			map[room.line + i][room.column + j].type = ROOM;
			map[room.line + i][room.column + j].state = LIGHT;
		}
	}
	return room;
}


/**
  * \brief choix d'un mur d'une pieces au hasard
  * \fn t_pos chooseRandomWall (t_room r)
  * \param r piece dont on veut un mur
  * \return la position d'une cellule qui est un mur de la piece
  */
t_pos chooseRandomWall (t_room r) {
	int isHorizontal = rand()%2;
	int isLeftOrTop = rand()%2;
	t_pos rep;
	if (isHorizontal) {
		if (isLeftOrTop) rep.line = r.line; // case horizontal top
		else rep.line = r.line + r.height - 1; // case horizontal bottom
		rep.column = r.column + randab(1, r.width - 1); // we don't want a angle
	}
	else {
		if (isLeftOrTop) rep.column = r.column; // case vertical left
		else rep.column = r.column + r.width - 1; // case vertical right
		rep.line = r.line + randab(1, r.height - 1); // here neither
	}
	return rep;
}

/**
  * \brief placement au hasard d'une porte fermée ou ouverte
  * \fn void putRandomDoor (t_cell map[][COLUMNS], t_pos pos)
  * \param map carte représenant l'étage
  * \param pos position ou on peut placer une porte
  */
void putRandomDoor (t_cell map[][COLUMNS], t_pos pos) {
	int doIt = rand()%3;
	if (doIt == 0) map[pos.line][pos.column].state = dOPEN;
	else if(doIt == 1) map[pos.line][pos.column].state = dCLOSE;
	else map[pos.line][pos.column].state = dNONE;
}


/* 	connect use an algorithm call flood and fill to search a path from a door to the corridor network,
		the principle is:
			1- write a zero on the starting point
			2- add it to the queue
			3- take a position in the queue and jump on it
			4- if you are not at the target position then
				4.1- read the value you are steping on
				4.2- add 1 to it
				4.3- for all the cell around you who (have not been visited AND can be), do
					4.3.1- write the new value on it
					4.3.2- add it to the queue
				4.4- go to step 3
			5. it's finish !!

		After it start from the end and move back to the start following the decreasing value.
*/

/**
  * \brief connecte une pièces au reste du réseau
  * \fn void connect(t_cell map[LINES][COLUMNS], int walkable[LINES][COLUMNS], t_room room)
  * \param map carte représenant l'étage
  * \param walkable le réseau actuel
  * \param room la pièce à connecter
  */
void connect(t_cell map[LINES][COLUMNS], int walkable[LINES][COLUMNS], t_room room) {
	int path[LINES][COLUMNS], i, j, l, c, val;
	int up = 0, down = 1, right = 2, left = 3, curDir, dir[4] = {0};
	t_pos head, start = chooseRandomWall (room);
	map[start.line][start.column].type = DOORWAY;
	putRandomDoor (map, start);

	for (i = 0; i < LINES; i++) for (j = 0; j < COLUMNS; j++) path[i][j] = -1;
	file_init();
	head = start;
	path[head.line][head.column] = 0;
	while (walkable[head.line][head.column] != 1) {
		l = head.line;
		c = head.column;
		val = path[l][c];
		if ( l+1 < LINES && path[l + 1][c] == -1 && (map[l + 1][c].type == EMPTY || map[l + 1][c].type == DOORWAY)) {
			path[l + 1][c] = val + 1;
			head.line = l + 1;
			file_ajouter (head);
		}
		if ( l-1 >= 0 && path[l - 1][c] == -1 && (map[l - 1][c].type == EMPTY || map[l - 1][c].type == DOORWAY)) {
			path[l - 1][c] = val + 1;
			head.line = l - 1;
			file_ajouter (head);
		}
		if ( c+1 < COLUMNS && path[l][c + 1] == -1 && (map[l][c + 1].type == EMPTY || map[l][c + 1].type == DOORWAY)) {
			path[l][c + 1] = val + 1;
			head.column = c + 1;
			head.line = l;
			file_ajouter (head);
		}
		if ( c-1 >= 0 && path[l][c - 1] == -1 && (map[l][c - 1].type == EMPTY || map[l][c - 1].type == DOORWAY)) {
			path[l][c - 1] = val + 1;
			head.column = c - 1;
			head.line = l;
			file_ajouter (head);
		}
		file_retirer(&head);
	}
	file_supprimer();

	while (head.line != start.line || head.column != start.column) {
		l = head.line;
		c = head.column;
		val = path[l][c] - 1;

		if (l + 1 <  LINES   && path[l + 1][c] == val) dir[down]  = 1;
		if (l - 1 >= 0       && path[l - 1][c] == val) dir[up]    = 1;
		if (c + 1 <  COLUMNS && path[l][c + 1] == val) dir[right] = 1;
		if (c - 1 >= 0       && path[l][c - 1] == val) dir[left]  = 1;

		for (i = 0; i < 4; i++) {
			if (dir[i] == 1) {
				curDir = i;
				dir[i] = 0;
			}
		}

		switch (curDir) {
			case 0 : l--; break;
			case 1 : l++; break;
			case 2 : c++; break;
			case 3 : c--; break;
		}

		walkable[l][c] = 1;
		head.line   = l;
		head.column = c;
	}

}


/**
  * \brief choisi quelles pieces lier
  * \fn void chooseLink (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom)
  * \param map carte représenant l'étage
  * \param rooms tableau contenant les pieces existantes
  * \param nbRoom nombre de pieces existantes
  */
void chooseLink (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int i, j, walkable[LINES][COLUMNS] = {{0}};

	t_pos doorPos = chooseRandomWall (rooms[0]);
	map[doorPos.line][doorPos.column].type = DOORWAY;
	putRandomDoor (map, doorPos);
	walkable[doorPos.line][doorPos.column] = 1;
	for (i = 1; i < nbRoom; i++) connect(map, walkable, rooms[i]);
	for (i = 0; i < LINES; i++)
		for (j = 0; j < COLUMNS; j++)
			if (walkable[i][j] == 1 && map[i][j].type == EMPTY) map[i][j].type = CORRIDOR;
}

/**
  * \brief choisi un endroit au hasard dans une pièce qui ne contient pas d'objets
  * \fn void randomFreePlace (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom, int iRoom, int * line, int * col)
  * \param map carte représenant l'étage
  * \param rooms tableau contenant les pieces existantes
  * \param nbRoom nombre de pieces existantes
  * \param iRoom index de la pieces que l'on veut, -1 si indifférent
  * \param line contiendra la ligne choisi
  * \param col contiendra la colonne choisi
  */
void randomFreePlace (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom, int iRoom, int * line, int * col) {
	if (!isBetween(iRoom, 0, nbRoom - 1)) iRoom = rand()%nbRoom;
	int secu = 0;
	do {
		*line = randab(1, rooms[iRoom].height - 1) + rooms[iRoom].line;
		*col = randab(1, rooms[iRoom].width - 1) + rooms[iRoom].column;
		secu++;
	} while (map[*line][*col].nbObject != 0 && secu < 100);
	if (map[*line][*col].nbObject != 0) *line = *col = -1;
}

/**
  * \brief vérifie si on est à côté d'une porte
  * \fn int nextToDoor (t_cell map[LINES][COLUMNS], int line, int column)
  * \param map Carte représenant l'étage
  * \param line Ligne choisi
  * \param column Colonne choisi
  */
int nextToDoor (t_cell map[LINES][COLUMNS], int line, int column){
		if(map[line+1][column].state==dNONE || map[line+1][column].state==dCLOSE || map[line+1][column].state==dOPEN) return TRUE;
		if(map[line-1][column].state==dNONE || map[line+1][column].state==dCLOSE || map[line+1][column].state==dOPEN) return TRUE;
		if(map[line][column+1].state==dNONE || map[line+1][column].state==dCLOSE || map[line+1][column].state==dOPEN) return TRUE;
		if(map[line][column-1].state==dNONE || map[line+1][column].state==dCLOSE || map[line+1][column].state==dOPEN) return TRUE;
		return FALSE;
}
/**
  * \brief place les différents objets
  * \fn void placeObject (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom)
  * \param map carte représenant l'étage
  * \param rooms tableau contenant les pieces existantes
  * \param nbRoom nombre de pieces existantes
  */
void placeObject (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	err("*** debut de placement Objet ***");
	int rEnterance = rand()%nbRoom, rExit, lineEn, colEn, lineEx, colEx;
	int lineFood, colFood;
	int lineTrap, colTrap;

	int i;

	do rExit = rand()%nbRoom; while (rEnterance == rExit);

	randomFreePlace (map, rooms, nbRoom, rEnterance, &lineEn, &colEn);
	randomFreePlace (map, rooms, nbRoom, rExit, &lineEx, &colEx);

	err("placement escalier vers le bas ...");
	map[lineEn][colEn].obj[map[lineEn][colEn].nbObject].type = STAIRS_DOWN;
	map[lineEn][colEn].obj[map[lineEn][colEn].nbObject].isDiscovered = DEBUG;

	err("et vers le haut !!");
	map[lineEx][colEx].obj[map[lineEn][colEn].nbObject].type = STAIRS_UP;
	map[lineEx][colEx].obj[map[lineEn][colEn].nbObject].isDiscovered = DEBUG;

	map[lineEn][colEn].nbObject++;
	map[lineEx][colEx].nbObject++;

	// Génération de la nourriture aléatoirement sur la carte
	for (i = 0; i < (int)(AV_NB_FOOD_ROOM * nbRoom); i++) {
		err("placement de nourriture !");
		randomFreePlace(map, rooms, nbRoom, -1, &lineFood, &colFood);
		map[lineFood][colFood].obj[map[lineFood][colFood].nbObject].type = FOOD;
		map[lineFood][colFood].obj[map[lineFood][colFood].nbObject].isDiscovered = DEBUG;

		map[lineFood][colFood].nbObject++;
	}

	// Génération des kits de santé aléatoirement sur la carte
	for (i = 0; i < (int)(AV_NB_MED_KIT_ROOM * nbRoom); i++) {
		err("placement des kits de santé !");
		randomFreePlace(map, rooms, nbRoom, -1, &lineFood, &colFood);
		map[lineFood][colFood].obj[map[lineFood][colFood].nbObject].type = MED_KIT;
		map[lineFood][colFood].obj[map[lineFood][colFood].nbObject].isDiscovered = DEBUG;

		map[lineFood][colFood].nbObject++;
	}

	// Génération des pièges aléatoirement sur la carte
	for (i = 0; i < (int)(AV_NB_TRAP_ROOM * nbRoom); i++) {
		err("placement d'un piege !");
	do{
		randomFreePlace(map, rooms, nbRoom, -1, &lineTrap, &colTrap);
	}while(nextToDoor(map,lineTrap, colTrap));
		map[lineTrap][colTrap].obj[map[lineTrap][colTrap].nbObject].type = TRAP;
		map[lineTrap][colTrap].obj[map[lineTrap][colTrap].nbObject].isDiscovered = DEBUG;
		map[lineTrap][colTrap].nbObject++;
	}

	err("*** fin de placement Objet ***");
}


/**
  * \brief créé un étage au hasard !
  * \fn void randomFloor (t_cell map[LINES][COLUMNS], int lvl)
  * \param map carte représenant l'étage
  * \param lvl hauteur de l'étage
  */
void randomFloor (t_cell map[LINES][COLUMNS], int lvl) {
	err("***Debut Random Floor*****************************************************");
	int nbRoom = randab (ROOM_NB_MIN + lvl, ROOM_NB_MAX + 1 + lvl), i;

	t_room rooms[ROOM_NB_MAX + NB_LVL];
	initFloor (map);

	for (i = 0; i < nbRoom; i++) {
		rooms[i] = randomRoom(map, rooms, i, &nbRoom);
	}
	chooseLink (map, rooms, nbRoom);

	placeObject (map, rooms, nbRoom);

	if (gLvlId < NB_LVL) {
		gLvl[gLvlId].nbRoom = nbRoom;
		for (i = 0; i < nbRoom; i++) {
			gLvl[gLvlId].rooms[i] = rooms[i];
		}
		gLvlId++;
	}
	err("***Fin Random Floor*******************************************************");
}
