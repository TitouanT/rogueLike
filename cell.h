#ifndef CELL_H
#define CELL_H

#include "bool.h"

typedef enum {EMPTY, WALL, ROOM, CORRIDOR, DOOR} t_type;

typedef struct {
	t_type type; // ROOM, CORRIDOR
	t_bool discovered;
} t_floor;

typedef enum {wV, wH, wUR, wUL, wDR, wDL} t_wallType;
typedef struct {
	t_type type; // WALL
	t_bool discovered;
	t_type wallType; //
} t_wall;

typedef enum {dNONE, dOPEN, dCLOSE} t_doorStates;
typedef struct {
	t_type type; // DOOR
	t_bool discovered;
	t_doorStates doorStates; // dNONE, dOPEN or dCLOSE
} t_door;


typedef union {
	t_type type; /* Commun a tous les type*/

	t_floor walk; // en attente d'un meilleur nom (floor est pris par le C)
	t_wall wall;
	t_door door;
} t_cell;

/* definition type d'un nouveau champ de l'union
typedef struct {
	t_type type; //
} t_;
*/

#endif /* end of include guard: CELL_H */
