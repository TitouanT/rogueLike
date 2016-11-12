#ifndef CELL_H
#define CELL_H

#define OBJ_NB_MAX 10

typedef enum {EMPTY, WALL, ROOM, CORRIDOR, DOORWAY} t_type;

typedef enum {objNONE, STAIRS_UP, STAIRS_DOWN} t_object; // il sera lpus long un jour :)

typedef enum {DEFAULT_STATE, dNONE, dOPEN, dCLOSE, LIGHT, DARK} t_state;


typedef struct {
	t_type type;
	t_state state;
	int isDiscovered;
	int nbObject;
	t_object obj[OBJ_NB_MAX];
} t_cell;

#endif /* end of include guard: CELL_H */
