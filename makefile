LIB = -lncurses
all:
	gcc -Wall -I ./include  src/main.c src/lvl.c src/filePos.c src/loadLvl.c src/display.c src/moves.c ${LIB} -o rogueLike
