#ifndef CELL_H
#define CELL_H

#include "mapConst.h"

typedef enum {EMPTY, WALL, ROOM, CORRIDOR, DOORWAY} t_type;

typedef enum {objNONE, STAIRS_UP, STAIRS_DOWN, FOOD} t_object; // il sera lpus long un jour :)

typedef enum {DEFAULT_STATE, dNONE, dOPEN, dCLOSE, LIGHT, DARK} t_state;


typedef struct {
	t_type type;
	t_state state;
	int isDiscovered;
	int nbObject;
	t_object obj[OBJ_NB_MAX];
} t_cell;

typedef struct {int line, column, height, width;} t_room;
typedef struct {
	int nbRoom;
	t_room rooms[ROOM_NB_MAX];
} t_lvl;

#endif /* end of include guard: CELL_H */
