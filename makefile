LIB = -lncurses
FLAGS = -Wall
DEBUG = -g

rogueLike: main.o lvl.o filePos.o loadLvl.o display.o moves.o interactions.o tools.o snake.o snake_list_ptr.o monstre.o
	gcc -o $@ ${FLAGS} ${DEBUG} $^ ${LIB}

main.o: src/main.c include/display.h include/cell.h include/character.h include/lvl.h include/mapConst.h include/moves.h include/interactions.h include/tools.h include/monstre.h
	gcc -c ${DEBUG} -I ./include $<

lvl.o: src/lvl.c include/filePos.h include/cell.h include/tools.h include/mapConst.h
	gcc -c -I ./include $<

filePos.o: src/filePos.c include/cell.h
	gcc -c -I ./include $<

loadLvl.o: src/loadLvl.c include/cell.h include/mapConst.h include/lvl.h include/moves.h include/tools.h include/character.h include/monstre.h
	gcc -c -I ./include $<

display.o: src/display.c include/cell.h include/mapConst.h include/monstre.h include/loadLvl.h include/tools.h include/character.h
	gcc -c -I ./include $<

moves.o: src/moves.c include/moves.h include/loadLvl.h include/character.h include/cell.h include/mapConst.h include/display.h include/tools.h
	gcc -c -I ./include $<

interactions.o: src/interactions.c include/cell.h include/character.h include/moves.h include/mapConst.h include/display.h include/loadLvl.h include/tools.h
	gcc -c -I ./include $<

tools.o: src/tools.c include/mapConst.h
	gcc -c -I ./include $<

monstre.o: src/monstre.c include/monstre.h include/tools.h include/cell.h include/lvl.h include/mapConst.h
	gcc -c -I ./include $<

snake.o: src/snake.c include/snake_list_ptr.h include/tools.h
	gcc -c -I ./include $<

snake_list_ptr.o: src/snake_list_ptr.c include/snake_list_ptr.h
	gcc -c -I ./include $<

# Compile les tests unitaires
test: tools.o
	gcc src/tools_tests.c $^ -o test.out -lcunit

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

mrproper: clean createFolders rogueLike
