/**
  * \file
  * \brief fonctions de génération d'étage
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#include <ncurses.h>
#include <string.h>
#include "cell.h"
#include "filePos.h"
#include "tools.h"

#define MIN(a,b) (a < b) ? a : b


/** tableau qui contient tous les étages de la partie */
t_lvl gLvl[NB_LVL];

/** permet de savoir quel étage est actuellement en cours de génération */
int gLvlId = NB_LVL;

/**
  * \brief initialise la gestion des informations sur les étages
  * \fn void initStatRoom (void)
  */
void initStatRoom () {
	gLvlId = 0;
}

/**
  * \brief donne les informations sur les étages
  * \fn void queryLvlData (t_lvl tabLvl[NB_LVL])
  * \param tabLvl tableau qui contiendra chaque niveau
  */
void queryLvlData (t_lvl tabLvl[NB_LVL]) {
	int i;
	if (gLvlId >= NB_LVL) {
		for (i = 0; i < NB_LVL; i++) {
			tabLvl[i] = gLvl[i];
		}
	}
}

/**
  * \brief écrit dans un fichier les données sur les étages
  * \fn void writeLvlData (t_lvl tabLvl[NB_LVL], char * fileName)
  * \param tabLvl tableau qui contient chaque niveau
  * \param fileName nom du fichier à écrire
  */
void writeLvlData (t_lvl tabLvl[NB_LVL], char * fileName) {
	FILE * file = fopen (fileName, "w");
	int i, j, line, column, height, width;
	for (i = 0; i < NB_LVL; i++) {
		fprintf (file, "%d ", tabLvl[i].nbRoom);
		for (j = 0; j < tabLvl[i].nbRoom; j++) {
			tabLvl[i].rooms[j].line = line;
			tabLvl[i].rooms[j].column = column;
			tabLvl[i].rooms[j].height = height;
			tabLvl[i].rooms[j].width = width;
			fprintf (file, "%d %d %d %d ", line, column, height, width);
		}
	}
}

/**
  * \brief lit dans un fichier les données sur les étages
  * \fn int readLvlData (t_lvl tabLvl[NB_LVL], char * fileName)
  * \param tabLvl tableau qui contiendra chaque niveau
  * \param fileName nom du fichier à lire
  * \return TRUE si la lecture c'est bien passée.
  * \return FALSE sinon.
  */
int readLvlData (t_lvl tabLvl[NB_LVL], char * fileName) {
	FILE * file = fopen (fileName, "r");
	if (file == NULL) return FALSE; // failure
	int i, j, nbRoom, line, column, height, width;
	for (i = 0; i < NB_LVL; i++) {

		fscanf (file, "%d", &nbRoom);
		tabLvl[i].nbRoom = nbRoom;

		for (j = 0; j < nbRoom; j++) {
			fscanf (file, "%d%d%d%d", &line, &column, &height, &width);
			tabLvl[i].rooms[j].line = line;
			tabLvl[i].rooms[j].column = column;
			tabLvl[i].rooms[j].height = height;
			tabLvl[i].rooms[j].width = width;
		}

	}
	return TRUE; // success
}

/**
  * \brief initialise l'étage a créer
  * \fn void initFloor (t_cell map[LINES][COLUMNS])
  * \param map matrice qui représente le niveau
  */
void initFloor (t_cell map[LINES][COLUMNS]) {
	int i, j;
	for (i = 0; i < LINES; i++) for (j = 0; j < COLUMNS; j++) {
		map[i][j].type = EMPTY;
		map[i][j].state = DEFAULT_STATE;
		map[i][j].isDiscovered = DEBUG;
		map[i][j].nbObject = 0;

	}
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
		maxHeight = MIN(LINES - room.line - 1, ROOM_MAX_HEIGHT);
		room.height = randab(ROOM_MIN_LEN, maxHeight + 1);

		maxWidth = MIN(COLUMNS - room.column - 1, ROOM_MAX_WIDTH);
		room.width  = randab(ROOM_MIN_LEN, maxWidth + 1);

		// if the room is touching another one then try another
		posNotOk = 0;
		for (i = 0; i < nbRoom && posNotOk == 0; i++)
			if (areInContact(room, rooms[i])) posNotOk = 1;

		acc++;
	} while (posNotOk && acc < maxTour); // maxTour is there to protect against infinite loop

	if (acc >= maxTour) { //seconde piece if acc reach maxTour then give up one room
		*nbTotal = nbRoom;
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
  * \fn void putRandomDoor (t_cell map[][COLUMNS], t_pos *pos)
  * \param map carte représenant l'étage
  * \param pos position ou on peut placer une porte
  */
void putRandomDoor (t_cell map[][COLUMNS], t_pos pos) {
	int doIt = rand()%3;
	if (doIt == 0) map[pos.line][pos.column].state = dOPEN;
	else if(doIt == 1) map[pos.line][pos.column].state = dCLOSE;
	else map[pos.line][pos.column].state = dNONE;
}

/**
  * \brief empeche le placement de deux porte qui se touche
  * \fn void avoidTouchingDoors (t_cell map[][COLUMNS], t_pos * pos)
  * \param map carte représenant l'étage
  * \param pos position d'une porte que l'on veut tester
  */
void avoidTouchingDoors (t_cell map[][COLUMNS], t_pos * pos) {
	if (map[pos->line + 1][pos->column].type != DOORWAY) {
		if (map[pos->line - 1][pos->column].type != DOORWAY) {
			if (map[pos->line][pos->column + 1].type != DOORWAY) {
				if (map[pos->line][pos->column - 1].type != DOORWAY) {
					// if all around you there are no doors, then you can be one!
					map[pos->line][pos->column].type = DOORWAY;
					putRandomDoor (map, *pos);
				}
				else (pos->column)--; // else you have to be in the shaddow of one surrounding you
			} else (pos->column)++;
		} else (pos->line)--;
	} else (pos->line)++;
}


/* 	createLink use an algorithm call flood and fill to search a path from a door to another,
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
  * \brief fait un couloir entre deux pieces
  * \fn void createLink (t_cell map[][COLUMNS], t_room r1, t_room r2)
  * \param map carte représenant l'étage
  * \param r1 premiere piece
  * \param r2 seconde piece
  */
void createLink (t_cell map[][COLUMNS], t_room r1, t_room r2) {
	int path[LINES][COLUMNS];
	int up = 0, down = 1, right = 2, left = 3, curDir;
	int i, j, val, c, l, dir[4] = {0};
	t_pos start = chooseRandomWall (r1), finish = chooseRandomWall (r2), head;

	for (i = 0; i < LINES; i++) for (j = 0; j < COLUMNS; j++) path[i][j] = -1;

	avoidTouchingDoors (map, &start);
	avoidTouchingDoors (map, &finish);

	file_init();
	head=start;
	path[head.line][head.column] = 0;

	while (head.line != finish.line || head.column != finish.column) {
		l = head.line;
		c = head.column;
		val = path[l][c];
		if ( l+1 < LINES && path[l + 1][c] == -1 && (map[l + 1][c].type == CORRIDOR || map[l + 1][c].type == EMPTY || map[l + 1][c].type == DOORWAY)) {
			path[l + 1][c] = val + 1;
			head.column = c;
			head.line = l + 1;
			file_ajouter (head);
		}
		if ( l-1 >= 0 && path[l - 1][c] == -1 && (map[l - 1][c].type == CORRIDOR || map[l - 1][c].type == EMPTY || map[l - 1][c].type == DOORWAY)) {
			path[l - 1][c] = val + 1;
			head.column = c;
			head.line = l - 1;
			file_ajouter (head);
		}
		if ( c+1 < COLUMNS && path[l][c + 1] == -1 && (map[l][c + 1].type == CORRIDOR || map[l][c + 1].type == EMPTY || map[l][c + 1].type == DOORWAY)) {
			path[l][c + 1] = val + 1;
			head.column = c + 1;
			head.line = l;
			file_ajouter (head);
		}
		if ( c-1 >= 0 && path[l][c - 1] == -1 && (map[l][c - 1].type == CORRIDOR || map[l][c - 1].type == EMPTY || map[l][c - 1].type == DOORWAY)) {
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

		if ( l + 1 <  LINES   && path[l + 1][c] == val ) dir[down]  = 1;
		if ( l - 1 >= 0       && path[l - 1][c] == val ) dir[up]    = 1;
		if ( c + 1 <  COLUMNS && path[l][c + 1] == val ) dir[right] = 1;
		if ( c - 1 >= 0       && path[l][c - 1] == val ) dir[left]  = 1;

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

		if (map[l][c].type != DOORWAY) {
			map[l][c].type = CORRIDOR;
		}
		head.line   = l;
		head.column = c;
	}

}
/*
methode:
	1. verifier si il y a un chemin direct.
	2. sinon, créé un chemin sans toucher de couloir
	3. sinon, se raccorder au chemin le plus proche
*/

/**
  * \brief test si un chemin existe deja entre deux pieces
  * \fn int isThereAnExistingPath (t_cell map[][COLUMNS], t_room r1, t_room r2)
  * \param map carte représenant l'étage
  * \param r1 premiere piece
  * \param r2 seconde piece
  * \return TRUE si un lien existe
  * \return FALSE sinon.
  */
int isThereAnExistingPath (t_cell map[][COLUMNS], t_room r1, t_room r2) {
	t_pos start, finish, head;
	int path[LINES][COLUMNS], l, c, seen = 1, notSeen = 0;

	for (l = 0; l < LINES; l++)
		for (c = 0; c < COLUMNS; c++)
			path[l][c] = notSeen;

	start.line = r1.line + 1;
	start.column = r1.column + 1;

	finish.line = r2.line + 1;
	finish.column = r2.column + 1;
	FILE * file = fopen ("err", "a");
	fprintf(file, "wall: %d, finish: %d\n", WALL, map[finish.line][finish.column].type);

	file_init();
	file_ajouter (start);
	path[start.line][start.column] = seen;
	path[finish.line][finish.column] = 4;
	while (file_est_vide() == 0 && path[finish.line][finish.column] != seen) {
		file_retirer(&head);
		l = head.line;
		c = head.column;

		if ( l+1 < LINES && path[l + 1][c] == notSeen && (map[l + 1][c].type == CORRIDOR || map[l + 1][c].type == ROOM || map[l + 1][c].type == DOORWAY)) {
			/*  if legal   AND  not already seen        AND (                                          walkable                                            )  */
			head.column = c;
			head.line = l + 1;
			path[l+1][c] = seen;
			file_ajouter (head);
		}
		if ( l-1 >= 0 && path[l - 1][c] == notSeen && (map[l - 1][c].type == CORRIDOR || map[l - 1][c].type == ROOM || map[l - 1][c].type == DOORWAY)) {
			head.column = c;
			head.line = l - 1;
			path[l - 1][c] = seen;
			file_ajouter (head);
		}
		if ( c+1 < COLUMNS && path[l][c + 1] == notSeen && (map[l][c + 1].type == CORRIDOR || map[l][c + 1].type == ROOM || map[l][c + 1].type == DOORWAY)) {
			head.column = c + 1;
			head.line = l;
			path[l][c + 1] = seen;
			file_ajouter (head);
		}
		if ( c-1 >= 0 && path[l][c - 1] == notSeen && (map[l][c - 1].type == CORRIDOR || map[l][c - 1].type == ROOM || map[l][c - 1].type == DOORWAY)) {
			head.column = c - 1;
			head.line = l;
			path[l][c - 1] = seen;
			file_ajouter (head);
		}
	}
	file_supprimer();
	int i, j;
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) fprintf (file, "%c", (path[i][j] == 4) ? 'X' : (path[i][j]) ? '#' : ' ');
		fprintf (file, "\n");
	}
	fprintf (file, "\n");
	fclose(file);
	if (path[finish.line][finish.column] == seen) return TRUE;
	else return FALSE;

}

/**
  * \brief choisi quelles pieces lier
  * \fn void chooseLink (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom)
  * \param map carte représenant l'étage
  * \param rooms tableau contenant les pieces existantes
  * \param nbRoom nombre de pieces existantes
  */
void chooseLink (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int i;
	for (i = 0; i < nbRoom-1; i++)
		if (isThereAnExistingPath(map, rooms[i], rooms[i+1]) == FALSE) // a direct path doesn't exist yet.
			createLink(map, rooms[i], rooms[i+1]);
		else while(1);

}

/**
  * \brief choisi un endroit au hasard dans une pièce qui ne contient pas d'objets
  * \fn void randomFreePlace (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom, int iRoom, int * line, int * col)
  * \param map carte représenant l'étage
  * \param rooms tableau contenant les pieces existantes
  * \param nbRoom nombre de pieces existantes
  * \param iRoom index de la pieces que l'on veut, -1 si indifférent
  * \param line contiendra la ligne choisi
  * \param colonne contiendra la colonne choisi
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
  * \brief place les différents objets
  * \fn void placeObject (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom)
  * \param map carte représenant l'étage
  * \param rooms tableau contenant les pieces existantes
  * \param nbRoom nombre de pieces existantes
  */
void placeObject (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int rEnterance = rand()%nbRoom, rExit, lineEn, colEn, lineEx, colEx;
	int lineFood, colFood;
	int lineTrap, colTrap;

	int i;

	do rExit = rand()%nbRoom; while (rEnterance == rExit);

	randomFreePlace (map, rooms, nbRoom, rEnterance, &lineEn, &colEn);
	randomFreePlace (map, rooms, nbRoom, rExit, &lineEx, &colEx);

	map[lineEn][colEn].obj[map[lineEn][colEn].nbObject].type = STAIRS_DOWN;
	map[lineEn][colEn].obj[map[lineEn][colEn].nbObject].isDiscovered = FALSE;
	map[lineEx][colEx].obj[map[lineEn][colEn].nbObject].type = STAIRS_UP;
	map[lineEx][colEx].obj[map[lineEn][colEn].nbObject].isDiscovered = FALSE;

	map[lineEn][colEn].nbObject++;
	map[lineEx][colEx].nbObject++;

	// Génération de la nourriture aléatoirement sur la carte
	for (i = 0; i < (int)AV_NB_FOOD_ROOM * nbRoom; i++) {
		randomFreePlace(map, rooms, nbRoom, -1, &lineFood, &colFood);
		map[lineFood][colFood].obj[map[lineFood][colFood].nbObject].type = FOOD;
		map[lineFood][colFood].obj[map[lineFood][colFood].nbObject].isDiscovered = FALSE;

		map[lineFood][colFood].nbObject++;
	}

	// Génération des pièges aléatoirement sur la carte
	for (i = 0; i < AV_NB_TRAP_ROOM * nbRoom; i++) {
		randomFreePlace(map, rooms, nbRoom, -1, &lineTrap, &colTrap);
		map[lineTrap][colTrap].obj[map[lineTrap][colTrap].nbObject].type = TRAP;
		map[lineTrap][colTrap].obj[map[lineTrap][colTrap].nbObject].isDiscovered = FALSE;
		map[lineTrap][colTrap].nbObject++;
	}


}


/**
  * \brief créé un étage au hasard !
  * \fn void randomFloor (t_cell map[LINES][COLUMNS], int lvl)
  * \param map carte représenant l'étage
  * \param lvl hauteur de l'étage
  */
void randomFloor (t_cell map[LINES][COLUMNS], int lvl) {
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
			gLvl[i].rooms[i] = rooms[i];
		}
		gLvlId++;
	}
}
