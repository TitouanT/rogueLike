#ifndef CELL_H
#define CELL_H

typedef enum {EMPTY, WALL, ROOM, CORRIDOR, DOOR} t_type;

typedef enum {objNONE, STAIRS_UP, STAIRS_DOWN} t_object; // il sera lpus long un jour :)
typedef struct {
	t_type type; // ROOM, CORRIDOR
	t_object object; // pour l'instant un seul objet par case apres on pourra mettre une liste. (on sait jamais ...)
	int isDiscovered;
	int isLight;
} t_floor;

typedef struct {
	t_type type; // WALL
	int isDiscovered;
} t_wall;

typedef enum {dNONE, dOPEN, dCLOSE} t_doorStates;
typedef struct {
	t_type type; // DOOR
	int isDiscovered;
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
