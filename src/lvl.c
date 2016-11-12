#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include "cell.h"
#include "mapConst.h"
#include "filePos.h"
#include "display.h"


/*Ces define ne sont pas dans nomVar.txt car local à ce fichier (lvl.c)*/
#define ROOM_NB_MIN 3
#define ROOM_NB_MAX 9
#define ROOM_MAX_HEIGHT 10
#define ROOM_MAX_WIDTH 20
#define ROOM_MIN_LEN 5
#define MIN(a,b) (a < b) ? a : b

typedef struct {int line, column, height, width, link[ROOM_NB_MAX], isLink;} t_room;


int readLvl (char * fileName, t_cell map[][COLUMNS]) { /// a mettre a jour
	int i, j, val;
	FILE * lvlFile = NULL;
	lvlFile = fopen (fileName, "r");
	if (lvlFile == NULL) return FALSE;
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			fscanf (lvlFile, "%d", &val);
			switch (val) {
				case EMPTY:
					map[i][j].type = EMPTY;
					break;

				case WALL:
					map[i][j].type = WALL;
					fscanf(lvlFile, "%d", &val);
					map[i][j].wall.isDiscovered = val;
					break;

				case ROOM:
					map[i][j].type = ROOM;
					fscanf (lvlFile, "%d", &val);
					map[i][j].walk.isDiscovered = val;
					fscanf (lvlFile, "%d", &val);
					map[i][j].walk.isLight = val;
					break;

				case CORRIDOR:
					map[i][j].type = CORRIDOR;
					fscanf (lvlFile, "%d", &val);
					map[i][j].walk.isDiscovered = val;
					fscanf (lvlFile, "%d", &val);
					map[i][j].walk.isLight = val;
					break;

				case DOOR:
					map[i][j].type = DOOR;
					fscanf (lvlFile, "%d", &val);
					map[i][j].door.isDiscovered = val;
					fscanf (lvlFile, "%d", &val);
					map[i][j].door.doorStates = val;
					break;
			}
		}
	}
	fclose(lvlFile);
	return TRUE;
}

void writeLvl (char * fileName, t_cell map[][COLUMNS]) { //// a mettre a jours
	int i, j;
	FILE * lvlFile = NULL;
	lvlFile = fopen (fileName, "w");
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			fprintf (lvlFile, "%d ", map[i][j].type);
			switch (map[i][j].type) {
				case EMPTY: break;

				case WALL:
					fprintf(lvlFile, "%d ", map[i][j].wall.isDiscovered);
					break;

				case ROOM:
				case CORRIDOR:
					fprintf (lvlFile, "%d ", map[i][j].walk.isDiscovered);
					fprintf (lvlFile, "%d ", map[i][j].walk.isLight);
					break;

				case DOOR:
					fprintf (lvlFile, "%d ", map[i][j].door.isDiscovered);
					fprintf (lvlFile, "%d ", map[i][j].door.doorStates);
					break;
			}
		}
		fprintf(lvlFile, "\n");
	}
	fclose(lvlFile);
}

void initFloor (t_cell map[LINES][COLUMNS]) {
	int i, j;
	for (i = 0; i < LINES; i++) for (j = 0; j < COLUMNS; j++) {
		map[i][j].type = EMPTY;
	}
}

int randab (int a, int b) { // return a number in [a,b[
	if (b <= a) {
		printf ("a = %d, b = %d\n", a, b);
		scanf("%*c");
	}
	return (rand()%(b-a)) + a;
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
			map[room.line + i][room.column + j].wall.isDiscovered = TRUE;

		}
		else {
			map[room.line + i][room.column + j].type = ROOM;
			map[room.line + i][room.column + j].walk.isDiscovered = TRUE;
			map[room.line + i][room.column + j].walk.isLight = TRUE;
			map[room.line + i][room.column + j].walk.object = objNONE;
		}
	}
	return room;
}

int minDist (t_room r1, t_room r2) { // calcul la distance entre le centre de deux pieces.
	int r1MidLine = r1.height / 2  + r1.line;
	int r1MidCol = r1.width / 2  + r1.column;
	int r2MidLine = r2.height / 2  + r2.line;
	int r2MidCol = r2.width / 2  + r2.column;
	int line = r1MidLine - r2MidLine;
	int col = r1MidCol - r2MidCol;
	if (line < 0) line = -line;
	if (col < 0) col = -col;
	return line + col;
}

// return the linear translation of value from [min, max] to [destMin, destMax]
int mapping (int value, int min, int max, int destMin, int destMax) {
	if (destMin > destMax) return destMax + destMin - mapping (value, min, max, destMax, destMin);
	return ((destMax - destMin) * (value - min) / (max - min)) + destMin;
				/*	max - min -> value - min
					destMax - destMin -> ? - destMin
					=> ? - destMin = (destMax - destMin) * (value - min) / (max - min)
					=> ? = ((destMax - destMin) * (value - min) / (max - min)) + destMin
				*/
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

void avoidTouchingDoors (t_cell map[][COLUMNS], t_pos * pos) {
	if (map[pos->line + 1][pos->column].type != DOOR) {
		if (map[pos->line - 1][pos->column].type != DOOR) {
			if (map[pos->line][pos->column + 1].type != DOOR) {
				if (map[pos->line][pos->column - 1].type != DOOR) {
					// if all around you there are no doors, then you can be one!
					map[pos->line][pos->column].type = DOOR;
					map[pos->line][pos->column].door.doorStates = dCLOSE;
					map[pos->line][pos->column].door.isDiscovered = TRUE;
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
		if ( l+1 < LINES && path[l + 1][c] == -1 && (map[l + 1][c].type == CORRIDOR || map[l + 1][c].type == EMPTY || map[l + 1][c].type == DOOR)) {
			path[l + 1][c] = val + 1;
			head.column = c;
			head.line = l + 1;
			file_ajouter (head);
		}
		if ( l-1 >= 0 && path[l - 1][c] == -1 && (map[l - 1][c].type == CORRIDOR || map[l - 1][c].type == EMPTY || map[l - 1][c].type == DOOR)) {
			path[l - 1][c] = val + 1;
			head.column = c;
			head.line = l - 1;
			file_ajouter (head);
		}
		if ( c+1 < COLUMNS && path[l][c + 1] == -1 && (map[l][c + 1].type == CORRIDOR || map[l][c + 1].type == EMPTY || map[l][c + 1].type == DOOR)) {
			path[l][c + 1] = val + 1;
			head.column = c + 1;
			head.line = l;
			file_ajouter (head);
		}
		if ( c-1 >= 0 && path[l][c - 1] == -1 && (map[l][c - 1].type == CORRIDOR || map[l][c - 1].type == EMPTY || map[l][c - 1].type == DOOR)) {
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
			if (map[l + 1][c].type != DOOR) {
				map[l + 1][c].type = CORRIDOR;
				map[l + 1][c].walk.isDiscovered = TRUE;
				map[l + 1][c].walk.isLight = FALSE;
				map[l + 1][c].walk.object = objNONE;
			}
			head.line = l + 1;
		}
		else if ( l-1 >= 0 && path[l - 1][c] == val ) {
			if (map[l - 1][c].type != DOOR) {
				map[l - 1][c].type = CORRIDOR;
				map[l - 1][c].walk.isDiscovered = TRUE;
				map[l - 1][c].walk.isLight = FALSE;
				map[l - 1][c].walk.object = objNONE;
			}
			head.line = l - 1;
		}
		else if ( c+1 < COLUMNS && path[l][c + 1] == val ) {
			if (map[l][c + 1].type != DOOR) {
				map[l][c + 1].type = CORRIDOR;
				map[l][c + 1].walk.isDiscovered = TRUE;
				map[l][c + 1].walk.isLight = FALSE;
				map[l][c + 1].walk.object = objNONE;
			}
			head.column = c + 1;
		}
		else if ( c-1 >= 0 && path[l][c - 1] == val ) {
			if (map[l][c - 1].type != DOOR) {
				map[l][c - 1].type = CORRIDOR;
				map[l][c - 1].walk.isDiscovered = TRUE;
				map[l][c - 1].walk.isLight = FALSE;
				map[l][c - 1].walk.object = objNONE;
			}
			head.column = c - 1;
		}
	}

}

void chooseLink (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int adj[ROOM_NB_MAX][ROOM_NB_MAX] = {{0}};
	int i, j;
	for (i = 0; i < nbRoom-1; i++) adj[i][i+1] = 1;
	for (i = 0; i < nbRoom - 1; i++) {
		for (j = i + 1; j < nbRoom; j++) {
			if (adj[i][j] == 1) createLink(map, rooms[i], rooms[j]);
		}
	}

}

void placeObject (t_cell map[LINES][COLUMNS], t_room * rooms, int nbRoom) {
	int rEnterance = rand()%nbRoom, rExit;
	do rExit = rand()%nbRoom; while (rEnterance == rExit);
	map[randab(1, rooms[rEnterance].height - 1) + rooms[rEnterance].line][randab(1, rooms[rEnterance].width - 1) + rooms[rEnterance].column].walk.object = STAIRS_DOWN;
	map[randab(1, rooms[rExit].height - 1) + rooms[rExit].line][randab(1, rooms[rExit].width - 1) + rooms[rExit].column].walk.object = STAIRS_UP;

}

void randomFloor (t_cell map[LINES][COLUMNS], int step) {
	int nbRoom = randab (ROOM_NB_MIN, ROOM_NB_MAX + 1), i;
	initFloor (map);
	if (step) {
		printf("Phase 0: initialisation de la map\n");
		//displayFloor(map);
		printf("Taper Enter pour continuer");
		scanf("%*c");
	}
	t_room rooms[ROOM_NB_MAX];
	for (i = 0; i < nbRoom; i++) {
		rooms[i] = randomRoom(map, rooms, i, &nbRoom);
		if (step) {
			printf("Phase 1.%d: création des pieces\n", i);
			//displayFloor(map);
			printf("Taper Enter pour continuer");
			scanf("%*c");
		}
	}
	chooseLink (map, rooms, nbRoom);
	placeObject (map, rooms, nbRoom);
	// for (i = 0; i < nbRoom - 1; i++) { // la proba que deux pieces soient connecté est inversement proportionnelle a la distance qui les sépare
	// 	for (j = i+1; j < nbRoom; j++) {
	// 		rooms[i].link[j] = rooms[j].link[i]= minDist(rooms[i], rooms[j]);
	// 	}
	// }
	//
	// for (i = 0; i < nbRoom; i++) {
	// 	min = max = rooms[i].link[(i == 0) ? 1 : 0]; // pour pas prendre une valeur qui n'existe pas
	// 	for (j = 0; j < nbRoom; j++) {
	// 		if (i != j) {
	// 			if (min > rooms[i].link[j]) min = rooms[i].link[j];
	// 			else if (max < rooms[i].link[j]) max = rooms[i].link[j];
	// 		}
	// 	}
	// 	for (j = 0; j < nbRoom; j++) {
	// 		if (i != j) {
	// 			if (min != max) rooms[i].link[j] = mapping (rooms[i].link[j], min, max, 100, 0);
	// 			else rooms[i].link[j] = 100;
	// 			if (rand()%101 <= rooms[i].link[j]) rooms[i].link[j] = 1;
	// 			else rooms[i].link[j] = 0;
	// 		}
	// 		else rooms[i].link[j] = 0;
	// 	}
	// }
	// for (i = 0; i < nbRoom; i++) rooms[i].isLink = 0;
	//
	// for (i = 0; i < nbRoom - 1; i++) {
	// 	for (j = i + 1; j < nbRoom; j++) {
	// 		if (rooms[i].link[j] && rooms[j].link[i]) {
	// 			createLink (map, rooms[i], rooms[j]);
	// 			rooms[i].isLink = rooms[j].isLink = 1;
	// 			if (step) {
	// 				printf("Phase 2.%d.%d: création des couloir\n", i, j);
	// 				//displayFloor(map);
	// 				printf("Taper Enter pour continuer");
	// 				scanf("%*c");
	// 			}
	// 		}
	// 	}
	// }
	//
	// for (i = 0; i < nbRoom; i++) {
	// 	if (rooms[i].isLink == 0) {
	// 		j = 0;
	// 		while (j < nbRoom && rooms[i].link[j] == 0) j++;
	// 		if (j == nbRoom) {
	//
	// 			do j=rand()%nbRoom; while (j==i);
	// 		}
	// 		createLink(map, rooms[i], rooms[j]);
	// 		if (step) {
	// 			printf("Phase 3.%d.%d: ajout de couloir\n", i, j);
	// 			//displayFloor(map);
	// 			printf("Taper Enter pour continuer");
	// 			scanf("%*c");
	// 		}
	// 	}
	// }


}
