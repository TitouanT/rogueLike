#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_HP 10
#define MAX_FOOD 100

typedef struct {
	char name[100];
	int line;
	int column;
	int lvl;
	int hp;
	int pw;
	int xp;
	int nbMove;
	int food;
	int isSick;
} t_character;

#endif /* end of include guard: CHARACTER_H */
