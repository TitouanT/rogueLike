LIB = -lncurses -lpanel
all:
	gcc -Wall -I ./include  src/main.c   src/lvl.c src/filePos.c src/display.c ${LIB}