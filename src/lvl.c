#include <ncurses.h>
#include <string.h>
#include "cell.h"
#include "mapConst.h"
#include "filePos.h"
#include "random.h"

#define DEBUG TRUE


/*Ces define ne sont pas dans nomVar.txt car local à ce fichier (lvl.c)*/
#define ROOM_NB_MIN 3//3
#define ROOM_NB_MAX 9//9
#define ROOM_MAX_HEIGHT 10
#define ROOM_MAX_WIDTH 20
#define ROOM_MIN_LEN 5
#define MIN(a,b) (a < b) ? a : b

typedef struct {int line, column, height, width, link[ROOM_NB_MAX], isLink;} t_room;
int globNbRoomTotal;


void initFloor (t_cell map[LINES][COLUMNS]) {
	int i, j;
	for (i = 0; i < LINES; i++) for (j = 0; j < COLUMNS; j++) {
		map[i][j].type = EMPTY;
		map[i][j].state = DEFAULT_STATE;
		map[i][j].isDiscovered = DEBUG;
		map[i][j].nbObject = 0;

	}
}

int isBetween (int val, int a, int b) { // return TRUE if val is in [a,b]
	if (a <= val && b >= val) return TRUE;
	else return FALSE;
}

int intervalOverlaping (int a, int b, int c, int d) { // return TRUE if the intersection of [a,b] and [c,d] is not empty
	if (isBetween (a, c, d) || isBetween (b, c, d) || isBetween (c, a, b) || isBetween (d, a, b)) return TRUE;
	else return FALSE;
}

int areInContact (t_room r1, t_room r2) { // return TRUE if the room are in Contact
	if (intervalOverlaping(r2.line, r2.line + r2.height, r1.line, r1.line + r1.height)
	&& intervalOverlaping(r2.column, r2.column + r2.width, r1.column, r1.column + r1.width)) return TRUE;
	else return FALSE;
}

t_room randomRoom (t_cell map[][COLUMNS], t_room * rooms, int nbRoom, int *nbTotal) {
	int maxHeight, maxWidth, i, j, posNotOk, maxTour = 100, acc;
	t_room room;
	acc = 0;
	do {
		// choose a random position for the top left corner of the room
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

	if (acc >= maxTour) { // if acc reach maxTour then give up one room
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

void putRandomRoom (t_cell map[][COLUMNS], t_pos *pos) {
	int doIt = rand()%3;
	if (doIt == 0) map[pos->line][pos->column].state = dOPEN;
	else if(doIt == 1) map[pos->line][pos->column].state = dCLOSE;
	else map[pos->line][pos->column].state = dNONE;
}

void avoidTouchingDoors (t_cell map[][COLUMNS], t_pos * pos) {
	if (map[pos->line + 1][pos->column].type != DOORWAY) {
		if (map[pos->line - 1][pos->column].type != DOORWAY) {
			if (map[pos->line][pos->column + 1].type != DOORWAY) {
				if (map[pos->line][pos->column - 1].type != DOORWAY) {
					// if all around you there are no doors, then you can be one!
					map[pos->line][pos->column].type = DOORWAY;
					putRandomRoom(map, pos);
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
void createLink (t_cell map[][COLUMNS], t_room r1, t_room r2) {
	int path[LINES][COLUMNS];
	int i, j, val, c, l;
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
		if ( l+1 < LINES && path[l + 1][c] == val ) {
			if (map[l + 1][c].type != DOORWAY) {
				map[l + 1][c].type = CORRIDOR;
			}
			head.line = l + 1;
		}
		else if ( l-1 >= 0 && path[l - 1][c] == val ) {
			if (map[l - 1][c].type != DOORWAY) {
				map[l - 1][c].type = CORRIDOR;
			}
			head.line = l - 1;
		}
		else if ( c+1 < COLUMNS && path[l][c + 1] == val ) {
			if (map[l][c + 1].type != DOORWAY) {
				map[l][c + 1].type = CORRIDOR;
			}
			head.column = c + 1;
		}
		else if ( c-1 >= 0 && path[l][c - 1] == val ) {
			if (map[l][c - 1].type != DOORWAY) {
				map[l][c - 1].type = CORRIDOR;
			}
			head.column = c - 1;
		}
	}

}

int isThereAnExistingPath (t_cell map[][COLUMNS], t_room r1, t_room r2) {
	t_pos start, finish, head;
	int path[LINES][COLUMNS], l, c, seen = 1, notSeen = 0;

	for (l = 0; l < LINES; l++) for (c = 0; c < COLUMNS; c++) path[l][c] = notSeen;

	start.line = r1.line + 1;
	start.column = r1.column + 1;

	finish.line = r2.line + 1;
	finish.column = r2.column + 1;

	file_init();
	file_ajouter (start);
	path[start.line][start.column] = seen;
	while (file_est_vide() == 0 && (file_retirer(&head), (head.line != finish.line || head.column != finish.column))) {
		l = head.line;
		c = head.column;

		if ( l+1 < LINES && path[l + 1][c] == notSeen && (map[l + 1][c].type == CORRIDOR || map[l + 1][c].type == ROOM || map[l + 1][c].type == DOORWAY)) {
/*       if legal   AND  not already seen  AND (                                          walkable                                            )  */
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
	if (head.line == finish.line && head.column == finish.column) return TRUE;
	else {
		//there some work to do there. That not because there is no direct path that we should made a totally new one.
		return FALSE;
	}




}

void chooseLink (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int adj[ROOM_NB_MAX][ROOM_NB_MAX] = {{0}};
	int i, j;
	for (i = 0; i < nbRoom-1; i++) adj[i][i+1] = 1;
	for (i = 0; i < nbRoom - 1; i++) {
		for (j = i + 1; j < nbRoom; j++) {
			if (adj[i][j] == 1 && isThereAnExistingPath(map, rooms[i], rooms[j]) == FALSE) createLink(map, rooms[i], rooms[j]);
		}
	}

}

void placeObject (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int rEnterance = rand()%nbRoom, rExit, lineEn, colEn, lineEx, colEx;

	do rExit = rand()%nbRoom; while (rEnterance == rExit);

	lineEn = randab(1, rooms[rEnterance].height - 1) + rooms[rEnterance].line;
	colEn = randab(1, rooms[rEnterance].width - 1) + rooms[rEnterance].column;

	lineEx = randab(1, rooms[rExit].height - 1) + rooms[rExit].line;
	colEx = randab(1, rooms[rExit].width - 1) + rooms[rExit].column;

	map[lineEn][colEn].obj[map[lineEn][colEn].nbObject] = STAIRS_DOWN;
	map[lineEx][colEx].obj[map[lineEn][colEn].nbObject] = STAIRS_UP;

	map[lineEn][colEn].nbObject++;
	map[lineEx][colEx].nbObject++;
}

void randomFloor (t_cell map[LINES][COLUMNS]) {
	int nbRoom = randab (ROOM_NB_MIN, ROOM_NB_MAX + 1), i;

	t_room rooms[ROOM_NB_MAX];
	initFloor (map);

	for (i = 0; i < nbRoom; i++) {
		rooms[i] = randomRoom(map, rooms, i, &nbRoom);
	}
	globNbRoomTotal += nbRoom;
	chooseLink (map, rooms, nbRoom);

	placeObject (map, rooms, nbRoom);
}

void initStatRoom() {
	globNbRoomTotal = 0;
}

int queryNbRoomTotal() {
	return globNbRoomTotal;
}
//
// int queryNbRoomLvl (int lvl) {
// 	if (isBetween())
// 	return
// }
