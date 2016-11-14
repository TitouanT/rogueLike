#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_HP 10

typedef struct {
	char name[100];
	int line;
	int column;
	int lvl;
	int hp;
	int pw;
	int xp;
} t_character;

#endif /* end of include guard: CHARACTER_H */
