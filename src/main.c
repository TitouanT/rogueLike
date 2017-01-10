/*! \mainpage rogueLike documentation
 *
 * Dans le cadre de notre projet en L2 SPI à l'université du Maine, nous avons réalisé un rogueLike. Le code se trouve à l'adresse suivante : <a href="https://github.com/TitouanT/rogueLike">https://github.com/TitouanT/rogueLike</a>
 * <img src="../include/screen_doxygen.png" style="width:100%;">
 * \section requirements_sec Programmes requis
 *
 * Pour utiliser notre jeu, vous aurez besoin de :
 *
 * - Compilateur `gcc`
 * - Constructeur `make`
 * - Librairie `ncurses`
 * - Gestionnaire `git`
 *
 *
 * \section installation_sec Installation
 *
 * Pour installer notre rogueLike, ouvrez un terminal, et entrez les commandes suivantes :
 *
 * \code{shell}
 * git clone https://github.com/TitouanT/rogueLike.git
 * cd rogueLike
 * make install
 * \endcode
 *
 *
 * \section lancement_sec Lancement du jeu
 *
 * \code{shell}
 * ./rogueLike
 * \endcode
 *
 *
 * \section jouer_sec Comment jouer ?
 *
 * - Pour se déplacer, utilisez les flèches de direction.
 * - Pour ouvrir une porte rouge, appuyez sur la touche `o` et signalez la direction de la porte avec les flèches de direction.
 * - Pour fermer une porte verte, appuyez sur la touche `f` et signalez la direction de la porte avec les flèches de direction.
 * - Pour passer à l'étage suivant ou à l'étage précédent, placez vous sur un escalier et appuyez sur la touche "entrée".
 * - Pour voir votre inventaire, appuyez sur la touche `i`.
 * - Pour récupérer un objet, placez-vous dessus, puis appuyez sur la touche `g`. Vous devez avoir un emplacement de libre dans votre inventaire.
 * - Pour lâcher un objet, appuyez sur la touche `d`, et indiquez le numéro de l'emplacement de l'objet que vous voulez lâcher.
 * - Pour sauvegarder le jeu, appuyez sur `s`.
 * - Pour quitter le jeu, appuyez sur `q`.
 * - Vous pouvez obtenir de l'aide en jeu en appuyant sur la touche `?`.
 *
 *
 */

/**
 * \file
 * \brief Programme principal du jeu.
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.2
 * \date 12 novembre 2016
 */

#include <locale.h>       // OK
#include <ncurses.h>      // OK
#include <stdlib.h>       // OK

#include "display.h"      // OK
#include "cell.h"         // OK
#include "character.h"    // OK
#include "lvl.h"          // OK
#include "mapConst.h"     // OK
#include "moves.h"        // OK
#include "interactions.h" // OK
#include "tools.h"        // OK
#include "monstre.h"      // OK

#include "math.h"

#define DIFFUSION_SIZE 20

int areDifferent(int l1, int c1, int l2, int c2) {
	return (l1 != l2 || c1 != c2);
}

int isAnObstacle (t_cell map[][COLUMNS], int l, int c) {
	return (map[l][c].type == WALL || (map[l][c].type == DOORWAY && map[l][c].state == dCLOSE));
}

void followLine (t_cell map[LINES][COLUMNS], int mat[][2*DIFFUSION_SIZE + 1], t_character player, int startLine, int startCol, int endLine , int endCol) {
	float sl = (float) startLine + 0.5;
	float sc = (float) startCol + 0.5;
	float el = (float) endLine + 0.5;
	float ec = (float) endCol + 0.5;
	float hl = sl;
	float hc = sc;
	float dirl = el - sl;
	float dirc = ec - sc;
	float mag = sqrt(dirl * dirl + dirc * dirc);
	dirl /= mag * 10;
	dirc /= mag * 10;
	while (areDifferent(endLine, endCol, (int) hl, (int) hc) && !isAnObstacle(map, (int) hl + player.line - startLine, (int) hc + player.column - startCol)) {
		mat[(int) hl][(int) hc] = 1;
		hl += dirl;
		hc += dirc;
	}
}

void lightDiffusion (t_cell map[LINES][COLUMNS], t_character player, int visibleByGhost[LINES][COLUMNS]) {
	int mat[2*DIFFUSION_SIZE + 1][2*DIFFUSION_SIZE + 1] = {{0}};
	int i, j, line, col;
	t_pos matTopLeft = {0, 0},
	      matBotRight = {2*DIFFUSION_SIZE, 2*DIFFUSION_SIZE},
	      center = {DIFFUSION_SIZE, DIFFUSION_SIZE},
	      matToVisibleByGhost;
	
	if (player.line - DIFFUSION_SIZE < 0) matTopLeft.line = -(player.line - DIFFUSION_SIZE);
	if (player.column - DIFFUSION_SIZE < 0) matTopLeft.column = -(player.column - DIFFUSION_SIZE);
	
	if (player.line + DIFFUSION_SIZE >= LINES) matBotRight.line -= (player.line + DIFFUSION_SIZE - (LINES-1));
	if (player.column + DIFFUSION_SIZE >= COLUMNS) matBotRight.column -= (player.column + DIFFUSION_SIZE - (COLUMNS-1));
	
	
	for (i = matTopLeft.column; i < matBotRight.column + 1; i++) {
		followLine(map, mat, player, center.line, center.column, matTopLeft.line , i);
		followLine(map, mat, player, center.line, center.column, matBotRight.line , i);
	}
	
	for (i = matTopLeft.line + 1; i < matBotRight.line; i++) {
		followLine(map, mat, player, center.line, center.column, i, matTopLeft.column);
		followLine(map, mat, player, center.line, center.column, i, matBotRight.column);
	}
	
	matToVisibleByGhost.line = player.line - center.line;
	matToVisibleByGhost.column = player.column - center.column;
	
	for (i = matTopLeft.line; i <= matBotRight.line; i++) {
		for (j = matTopLeft.column; j <= matBotRight.column; j++) {
			if (mat[i][j] == 1) {
				line = i + matToVisibleByGhost.line;
				col = j + matToVisibleByGhost.column;
				if (/*map[line][col].type == DOORWAY ||*/ map[line][col].type == ROOM) visibleByGhost[line][col] = 1;
			}
		}
	}
}

int main () {

	if (fileExist("./accentNeeded")) setlocale(LC_ALL, "");

	initErr();
	err ("<main>", +1);

	int key;
	int i, j;
	int lineLog;
	int widthScreen, heightScreen;
	int continueGame = TRUE;
	int nbMonster;
	int visibleByGhost[LINES][COLUMNS];
	int isPlayerInvicible;
	int wasNewGame;
	t_cell map[LINES][COLUMNS];
	t_monster monsters[NB_MONSTER_MAX];
	t_character player;


	err ("main: initialisations", 0);

	initRandom();

	init_screen();
	getmaxyx(stdscr,heightScreen,widthScreen);

	err ("main: Affichage du screen de départ", 0);
	WINDOW *win_start = newwin(heightScreen, widthScreen, 0, 0);
	startScreen(win_start);

	deleteWindow(win_start);


	do{
		// Mise à zéro de toutes les variables, utile lorsque le joueur relance une partie
		player = (t_character){"root", 0, 0, 0, 10, 10, 60, 10, 0, MAX_FOOD, FALSE, FALSE, {objNONE}};
		lineLog = 0;
		isPlayerInvicible = FALSE;
		for(i = 0 ; i < LINES ; i++){
			for(j = 0 ; j < COLUMNS ; j++){
				visibleByGhost[i][j] = 0;
			}
		}

		err ("Affichage du screen de choix", 0);
		WINDOW *win_choice = newwin(heightScreen, widthScreen, 0, 0);
		wasNewGame = selectionScreen(win_choice, map, &player, monsters, &nbMonster);

		deleteWindow(win_choice);

		err ("Affichage du screen avec map, logs et stats", 0);
		/* Initialisation des fenetres */
		WINDOW *win_game  = createWindow(0, 0, COLS_GAME, LINES_GAME, "RogueLike");
		WINDOW *win_stats = createWindow(0, LINES_GAME, COLS_STATS, LINES_STATS, "Statistiques");
		WINDOW *win_logs  = createWindow(COLS_GAME, 0, COLS_LOGS, LINES_LOGS, "Logs");

		// On affiche les objectifs
		if(wasNewGame) displayObjectives(&lineLog, win_logs);
		// On affiche la map et le joueur
		lightDiffusion(map, player, visibleByGhost);
		displayFloor(map, player, win_game, visibleByGhost);
		displayPlayer(player, map, win_game, win_logs, &lineLog);
		displayMonster (win_game, monsters, map, nbMonster, player.lvl, visibleByGhost);
		displayStats(player, win_stats, isPlayerInvicible);


		err ("ENTREE DANS LA BOUCLE DU JEU", +1);

		/* Ici se déroule tout le jeu */

		while (continueGame && player.hp > 0) {

			err ("debut d'un tour de jeu", +1);
			key = getch();
			if(isPlayerInvicible){
				player.hp   = 10000;
				player.food = 10000;
			}

			if (key == 'N') {
				err("cheat N pour une nouvelle map", 0);
				randomFloor(map, 6);
				displayFloor(map, player, win_game, visibleByGhost);
				move2spawn(map, &player, STAIRS_DOWN);
				displayPlayer(player, map, win_game, win_logs, &lineLog);
				err("fin du cheat N", 0);
			}

			clearLog(&lineLog, win_logs);
			continueGame = handleInteraction(key, map, &player, win_logs, win_game, &lineLog, monsters, nbMonster, &isPlayerInvicible, visibleByGhost);
			markDiscoverRoom(map, player);

			//moveMonster(map, monsters, nbMonster, &player);
			setVisibleByGhost (monsters, visibleByGhost, player);
			lightDiffusion(map, player, visibleByGhost);

			err("affichage etage, player, stats", 0);
			displayFloor(map, player, win_game, visibleByGhost);
			displayPlayer(player, map, win_game, win_logs, &lineLog);
			displayMonster (win_game, monsters, map, nbMonster, player.lvl, visibleByGhost);
			displayStats(player, win_stats, isPlayerInvicible);
			err("fin affichage etage, player, stats", 0);

			err ("fin d'un tour de jeu", -1);

		}
		err ("SORTIE DE LA BOUCLE DU JEU", -1);



		// ----- UNE FOIS QUE LE JEU EST TERMINE ----- //

		// Si le joueur est sorti du jeu sans vouloir quitter manuellement
		if(key != 'q' && key != 'Q'){
			err ("Affichage du gameOver", 0);
			WINDOW *win_end = newwin(heightScreen, widthScreen, 0, 0);
			displayEnd(player, win_end);
			while((key = getch()) != 'q' && key != 'Q' && up2low(key) != 'r');
			err ("Fin Affichage du gameOver", 0);
		}
	}while(key != 'q' && key != 'Q');

	endwin(); //Fermeture de la fenetre
	err ("</main>", -1);
	unInitErr();
	return EXIT_SUCCESS;
}
