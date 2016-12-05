LIB = -lncurses
FLAGS = -Wall

# all:
# 	gcc -Wall -I ./include  src/main.c src/lvl.c src/filePos.c src/loadLvl.c src/display.c src/moves.c -lncurses -o rogueLike

rogueLike: main.o lvl.o filePos.o loadLvl.o display.o moves.o interactions.o tools.o snake.o snake_list_ptr.o
	gcc -o $@ ${FLAGS} $^ ${LIB}

main.o: src/main.c include/global.h                                                      include/cell.h include/character.h include/mapConst.h include/lvl.h include/display.h include/moves.h include/loadLvl.h include/tools.h
	gcc -c -I ./include $<

lvl.o: src/lvl.c include/filePos.h include/cell.h include/filePos.h include/tools.h include/mapConst.h
	gcc -c -I ./include $<

filePos.o: src/filePos.c include/filePos.h
	gcc -c -I ./include $<

loadLvl.o: src/loadLvl.c include/global.h                                                 include/cell.h include/character.h include/mapConst.h include/lvl.h include/display.h include/moves.h include/loadLvl.h include/tools.h
	gcc -c -I ./include $<

display.o: src/display.c include/global.h include/display.h                               include/cell.h include/character.h include/mapConst.h include/lvl.h include/display.h include/moves.h include/loadLvl.h include/tools.h
	gcc -c -I ./include $<

moves.o: src/moves.c include/global.h include/filePos.h                                   include/cell.h include/character.h include/mapConst.h include/lvl.h include/display.h include/moves.h include/loadLvl.h include/tools.h
	gcc -c -I ./include $<

interactions.o: src/interactions.c include/global.h include/tools.h                      include/cell.h include/character.h include/mapConst.h include/lvl.h include/display.h include/moves.h include/loadLvl.h include/tools.h
	gcc -c -I ./include $<

tools.o: src/tools.c include/tools.h
	gcc -c -I ./include $<

snake.o: src/snake.c include/snake_list_ptr.h
	gcc -c -I ./include $<

snake_list_ptr.o: src/snake_list_ptr.c include/snake_list_ptr.h
	gcc -c -I ./include $<

# ce qui suit n'est utile que pour garder en mémoire les dépendances des header
include/cell.h:

include/character.h:

include/display.h: include/cell.h include/character.h include/mapConst.h

include/filePos.h:

include/global.h: include/cell.h include/character.h include/mapConst.h include/lvl.h include/display.h include/moves.h include/loadLvl.h include/tools.h

include/loadLvl.h:

include/lvl.h: include/cell.h include/mapConst.h

include/mapConst.h:

include/moves.h:

include/interactions.h:

include/tools.h:


# Permet de mettre à jour automatiquement le jeu
upgrade: createFolders
	git pull
	make

# Pour l'installation, on commence par supprimer les données du jeu
install: createFolders
	make

createFolders: clean
	mkdir partie
	mkdir partie/sauvegarde1
	mkdir partie/sauvegarde2
	mkdir partie/sauvegarde3

clean:
	rm -rf *.o rogueLike
	rm -rf partie
clear: clean
