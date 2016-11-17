#ifndef MONSTER_H
#define MONSTER_H

#include "cell.h"
#include "mapConst.h"

#define NB_MONSTER_MAX 50
#define NB_MONSTER_MIN 20

typedef enum {L1, L2, L3, MASTER, DOC, GHOST} t_type_monster;

typedef struct {
	t_type_monster type;
	char name[100];

	int line;
	int col;
	int lvl;

	int hp;
	int pw;
	int speed;

	int data1;
	int data2;
	int data3;
} t_monster;





#endif /* end of include guard: MONSTER_H */
