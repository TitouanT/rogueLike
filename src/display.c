/**
	* \file
	* \brief Fonctions gérant l'affichage (utilisation de ncurses)
	* \author MOTTIER Emeric
	* \author PELLOIN Valentin
	* \author TEYSSIER Titouan
	* \date 22 novembre 2016
	* \version 1.0
	*/

#include <ncurses.h>   // OK
#include <string.h>    // OK
#include <stdlib.h>    // OK

#include "cell.h"      // OK
#include "mapConst.h"  // OK
#include "monstre.h"   // OK
#include "tools.h"     // OK
#include "loadLvl.h"   // OK
#include "character.h" // OK

int konami (int key);

/***********************************************/
/* Definition des constantes pour les couleurs */
/***********************************************/
/** Paire de couleur générale */
#define GENERAL_COLOR   1

/** Paire de couleur des couloirs */
#define CORRIDOR_COLOR  2

/** Paire de couleur des murs */
#define WALL_COLOR      3

/** Paire de couleur des pièces */
#define ROOM_COLOR      4

/** Paire de couleur du joueur */
#define PLAYER_COLOR    5

/** Paire de couleur des objets */
#define OBJECTS_COLOR   6

/** Paire de couleur d'une porte ouverte */
#define OPENED_DOOR     7

/** Paire de couleur du joueur dans un couloir */
#define OBJECT_IN_CORRIDOR  8

/** Paire de couleur de la barre verte */
#define BAR_GREEN       9

/** Paire de couleur de la barre rouge */
#define BAR_RED        10

/** Paire de couleur du titre */
#define COLOR_TITLE    11

/** Paire de couleur des kits de santé */
#define MED_KIT_COLOR  12

/** Paire de couleur des kits de santé dans les couloirs*/
#define MED_KIT_COLOR_C 13

/** Paire de couleur de la barre jaune */
#define BAR_YELLOW      14


/** Touche retour en arrière (celle de ncurses ne fonctionne pas) */
#define KEY_RETURN 263
#define KEY_RETURN_MAC 127


/**
	* \brief Initialisation de toutes les couleurs utilisées par le jeu
	*	\fn void init_colors()
	*/
void init_colors(){
	err ("<init_colors>", +1);

	start_color();

	init_pair(GENERAL_COLOR , COLOR_RED,   COLOR_BLACK);
	init_pair(CORRIDOR_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(WALL_COLOR    , COLOR_CYAN,  COLOR_BLACK);
	init_pair(ROOM_COLOR    , COLOR_WHITE, COLOR_WHITE);
	init_pair(PLAYER_COLOR  , COLOR_GREEN, COLOR_WHITE);
	init_pair(OBJECTS_COLOR , COLOR_BLACK, COLOR_WHITE);
	init_pair(OPENED_DOOR   , COLOR_WHITE, COLOR_GREEN);
	init_pair(OBJECT_IN_CORRIDOR, COLOR_GREEN, COLOR_BLACK);

	init_pair(BAR_GREEN     , COLOR_GREEN, COLOR_GREEN);
	init_pair(BAR_RED       , COLOR_RED  ,   COLOR_RED);
	init_pair(BAR_YELLOW    , COLOR_YELLOW , COLOR_YELLOW);
	init_pair(COLOR_TITLE   , COLOR_GREEN, COLOR_BLACK);
	init_pair(MED_KIT_COLOR , COLOR_RED,   COLOR_WHITE);
	init_pair(MED_KIT_COLOR_C, COLOR_RED, COLOR_BLACK);

	err ("</init_colors>", -1);
}

/**
	* \brief Ferme le jeu
	*	\fn void abortGame()
	*/
void abortGame(){
	err ("<abortGame>", +1);
	err("on quitte le jeu, la fonction abortgame() a ete appelee", 0);
	endwin(); //Fermeture de la fenetre
	err("good bye", 0);
	err ("</abortGame>", -1);
	exit(EXIT_SUCCESS);
}

/**
	* \brief Initialisation de l'écran du jeu.
	* Affiche un message d'avertissement et quitte le jeu si la fenêtre est trop petite.
	*	\fn void init_screen()
	*/
void init_screen(){
	err ("<init_screen>", +1);

	int lines, columns;

	initscr();
	init_colors();

	keypad(stdscr, TRUE); // Pour ne pas afficher les lettres que l'utilisateur tape
	noecho();
	curs_set(0);

	getmaxyx(stdscr,lines,columns);

	if(columns < COLS_GAME + COLS_LOGS || lines < LINES_GAME + LINES_STATS){

		char * tooSmall = "Votre fenêtre est trop petite ! Elle doit faire au minimum :";

		mvprintw(lines / 2 - 1, (columns - strlen(tooSmall)) / 2, "%s", tooSmall);
		mvprintw(lines / 2 + 1, (columns - 8) / 2, "%i x %i", COLS_GAME+COLS_LOGS, LINES_GAME+LINES_STATS);

		getch();
		abortGame();
	}

	refresh();
	err ("</init_screen>", -1);
}

/**
* \brief Affichage une ligne centrée horizontalement
*	\fn void printLineCenter(char *msg, int widthScreen, int line, WINDOW *win)
* \param msg Message à afficher
* \param widthScreen Largeur de l'écran
* \param line Ligne sur laquelle on veut écrire le message
* \param win Fenêtre où afficher le message
*/
void printLineCenter(char *msg, int widthScreen, int line, WINDOW *win){
	err ("<printLineCenter>", +1);

	mvwprintw(win, line, (widthScreen - strlen(msg)) / 2, "%s", msg);

	err ("</printLineCenter>", -1);
}

/**
	* \brief Afficher le contenu d'un fichier
	* Permet d'afficher des messages ASCII
	*	\fn void printASCIIText(char * file, int * line, int xShift, WINDOW *win)
	* \param file Fichier ASCII à afficher
	* \param line Ligne où afficher le message
	* \param xShift Décalage horizontal à partir de la gauche
	* \param win Fenetre où afficher le message
	*/
void printASCIIText(char * file, int * line, int xShift, WINDOW *win){
	err ("<printASCIIText>", +1);
	FILE *texte;
	char letter;

	texte = fopen(file, "r");

	if(texte != NULL) {

		fscanf(texte, "%c", &letter);

		while(!feof(texte)){
			if(letter == '\n') wmove(win, (*line)++, xShift);
			else wprintw(win, "%c", letter);
			fscanf(texte, "%c", &letter);
		}
		fclose(texte);
	}
	err ("</printASCIIText>", -1);
}

/**
	* \brief Afficher le contenu d'un fichier au centre de l'écran
	*	\fn void printASCIICenter(char *file, char * message, char messageTwo, WINDOW * win)
	* \param file Fichier ASCII à afficher
	* \param message Message à afficher en dessous
	* \param messageTwo Deuxième message à afficher
	* \param win Fenetre où afficher le message
	*/
void printASCIICenter(char *file, char * message, char * messageTwo, WINDOW * win){
	err ("<printASCIICenter>", +1);

	int lines, columns;
	int yShift, xShift;
	getmaxyx(win, lines, columns);

	wattron(win, COLOR_PAIR(COLOR_TITLE));

	yShift = (lines - numberLinesFile(file)) / 2;
	xShift = (columns - maxColsFile(file)) / 2;

	wmove(win, yShift++, xShift);
	printASCIIText(file, &yShift, xShift, win);

	printLineCenter(message, columns, yShift+2, win);
	printLineCenter(messageTwo, columns, yShift+3, win);
	err ("</printASCIICenter>", -1);

}

/**
	* \brief Affichage de l'écran de départ du jeu
	*	\fn void startScreen(WINDOW *win)
	* \param win Fenêtre à utiliser pour l'affichage des informations
	*/
void startScreen(WINDOW *win){

	err ("<startScreen>", +1);
	int lines, columns;
	getmaxyx(win, lines, columns);

	printASCIICenter("include/logo.txt", "Appuyez sur une touche pour jouer.", "", win);

	mvwprintw(win, lines - 1, 0, "Roguelike créé par MOTTIER Emeric - PELLOIN Valentin - TEYSSIER Titouan.");
	wrefresh(win);
	getch();
	err ("</startScreen>", -1);
}

/**
* \brief Efface le contenu d'un zone d'une fenêtre
*	\fn void clearArea(WINDOW *win, int startX, int startY, int width, int height)
* \param win Nom de la fenêtre à affecter
* \param startX Décalage horizontal de la zone à effacer (en partant du point 0 à gauche)
* \param startY Décalage vertical de la zone à effacer (en partant du point 0 à en haut)
* \param width Largeur de la zone à effacer
* \param height Hauteur de la zone à effacer
*/
void clearArea(WINDOW *win, int startX, int startY, int width, int height){
	err ("<clearArea>", +1);

	int i, j;

	for(i = startY ; i < height ; i++){
		for(j = startX ; j < width ; j++){
			mvwprintw(win, i, j, " ");
		}
	}
	wrefresh(win);

	err ("</clearArea>", -1);
}

/**
	* \brief Affichage une boîte de taille et de coordonnées données
	*	\fn void drawBox(int startX, int startY, int sizeX, int sizeY, WINDOW *win, char color)
	* \param startX Position X de départ de la boîte
	* \param startY Position Y de départ de la boîte
	* \param sizeX Largeur de la boîte
	* \param sizeY Hauteur de la boîte
	* \param win Fenêtre où afficher la boîte
	* \param color Couleur de la boîte (r pour rouge, w pour blanc)
	*/
void drawBox(int startX, int startY, int sizeX, int sizeY, WINDOW *win, char color){

	err ("<drawBox>", +1);
	int x = startX, y = startY;
	int i;

	clearArea(win, startX, startY, sizeX, sizeY);

	switch (color) {
		case 'r' : wattron(win, COLOR_PAIR(OBJECT_IN_CORRIDOR));  break;
		case 'w' : wattroff(win, COLOR_PAIR(OBJECT_IN_CORRIDOR)); break;
	}

	mvwaddch(win, y, x, ACS_ULCORNER);
	for(x++ ; x-startX < sizeX ; x++) mvwaddch(win, y, x, ACS_HLINE);
	mvwaddch(win, y, x, ACS_URCORNER);

	while(y-startY <= sizeY){

		y++;
		x = startX;

		mvwaddch(win, y, x, ACS_VLINE);
		for(x++ ; x-startX < sizeX ; x++) mvwaddch(win, y, x, ' ');
		mvwaddch(win, y, x, ACS_VLINE);
	}

	y++;
	x = startX;

	mvwaddch(win, y, x, ACS_LLCORNER);
	for(x++ ; x-startX < sizeX ; x++) mvwaddch(win, y, x, ACS_HLINE);
	mvwaddch(win, y, x, ACS_LRCORNER);


	err ("</drawBox>", -1);
}

/**
	* \brief Affichage du contenu d'une sauvegarde
	*	\fn void printSaveInfos(WINDOW *win, int saveNB, int selectedGame)
	* \param win Fenêtre où afficher les informations
	* \param saveNB Numéro de la sauvegarde
	* \param selectedGame Numéro du jeu sélectionné
	*/
void printSaveInfos(WINDOW *win, int saveNB, int selectedGame){
	err ("<printSaveInfos>", +1);

	int lines, columns;
	char color = 'w';
	getmaxyx(win,lines,columns);

	int boxWidth = 50, boxHeight = 5;
	int center = (columns-boxWidth)/2;
	int topShift = saveNB * 10;
	int leftShift = center;

	if(saveNB == selectedGame) color = 'r';

	drawBox(center, topShift, boxWidth, boxHeight, win, color);

	mvwprintw(win, topShift+1, leftShift+1, "Sauvegarde n°%i : ", saveNB);

	wattron(win, COLOR_PAIR(GENERAL_COLOR));
	if(bFileSaveEmpty(saveNB)==TRUE){
		mvwprintw(win, topShift+2, leftShift+1, "     Emplacement vide", saveNB);
	}else{
		mvwprintw(win, topShift+2, leftShift+1, "     Reprendre la partie", saveNB);
	}

	wattroff(win, COLOR_PAIR(GENERAL_COLOR));


	err ("</printSaveInfos>", -1);
}

/**
	* \brief Gère l'affichage de l'écran de sélection de la sauvegarde
	*	\fn int selectionScreen(WINDOW *win, t_cell map[LINES][COLUMNS], t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonster)
	* \param win Fenêtre où afficher les informations
	* \param map Carte du joueur
	* \param player Infos du joueur
	* \param monsters L'ensemble des monstres du jeu
	* \param nbMonster Nombre de monstres dans le jeu
	* \return TRUE si une nouvelle partie a été créée
	* \return FALSE sinon
	*/
int selectionScreen(WINDOW *win, t_cell map[LINES][COLUMNS], t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonster){
	err ("<selectionScreen>", +1);

	int lines, columns;
	getmaxyx(win,lines,columns);

	char message[100];
	int selectedGame = 2;
	int key;
	int quit = FALSE;
caCEstDuPropre:
	err("Affichage de l'écran de sélection de partie ", 0);

	printLineCenter("Choisissez un emplacement de sauvegarde :", columns, 5, win);
	printLineCenter("(utiliser les flèches)", columns, 6, win);


	printLineCenter("Entrée : Valider               ", columns, 40, win);
	printLineCenter("Del    : Supprimer la sauvegarde", columns, 41, win);
	printLineCenter("q      : Sortir du jeu          ", columns, 42, win);


	while(quit == FALSE){
		if (selectedGame == 4) selectedGame = 1;
		else if (selectedGame == 0) selectedGame = 3;

		printSaveInfos(win, 1, selectedGame);
		printSaveInfos(win, 2, selectedGame);
		printSaveInfos(win, 3, selectedGame);

		wrefresh(win);

		key = getch();

		if(konami(key)) goto caCEstDuPropre; // hehe :p

		switch (key) {
			case '\n'           : quit = TRUE; break;
			case 'q'            : abortGame();
			case KEY_RETURN     :
			case KEY_RETURN_MAC : deleteGame(selectedGame); break;

			case KEY_UP     : /*if(selectedGame >= 2)*/ selectedGame--; break;
			case KEY_DOWN   : /*if(selectedGame <= 2)*/ selectedGame++; break;

			default : break;
		}
	}

	if(bFileSaveEmpty (selectedGame) == FALSE ){
		initGameMap (map, CONTINUE_GAME, selectedGame, player, monsters, nbMonster);
		err ("</selectionScreen>", -1);
		return FALSE;
	}
	else {
		initGameMap (map, NEW_GAME, selectedGame, player, monsters, nbMonster);
		err ("</selectionScreen>", -1);
		return TRUE;
	}


}

/**
	* \brief affiche les monstres sur la map.
	* \fn void displayMonster (WINDOW * win, t_monster monsters[NB_MONSTER_MAX], t_cell map[LINES][COLUMNS], int nbMonster, int currentLvl, int visibleByGhost[LINES][COLUMNS])
	* \param win fenêtre où afficher les monstres
	* \param monsters L'ensemble des monstres du jeu
	* \param map Carte du joueur
	* \param nbMonster Nombre de monstres dans le jeu
	* \param currentLvl niveau du joueur
	* \param visibleByGhost matrice pour savoir ce que les fantomes rendent visible.

	*/
void displayMonster (WINDOW * win, t_monster monsters[NB_MONSTER_MAX], t_cell map[LINES][COLUMNS], int nbMonster, int currentLvl, int visibleByGhost[LINES][COLUMNS]) {
	err ("<displayMonster/>", 0);

	int i;
	for (i = 0; i < nbMonster; i++) {
		if (monsters[i].hp > 0 && monsters[i].lvl == currentLvl && (map[monsters[i].line][monsters[i].col].isDiscovered || visibleByGhost[monsters[i].line][monsters[i].col] == 1)) {
			if(map[monsters[i].line][monsters[i].col].type == ROOM){
				wattron(win, COLOR_PAIR(PLAYER_COLOR));
			}
			else wattron(win, COLOR_PAIR(OBJECT_IN_CORRIDOR));

			wmove(win, (monsters[i].line)+1, (monsters[i].col)+1);
			switch (monsters[i].type) {
				case L1:     wprintw(win, "1"); break;
				case L2:     wprintw(win, "2"); break;
				case L3:     wprintw(win, "3"); break;
				case MASTER: wprintw(win, "M"); break;
				case DOC:    wprintw(win, "D"); break;
				case GHOST:  wprintw(win, "G"); break;
			}
			wrefresh(win);
		}
	}
	//err ("</displayMonster>", -1);
}

/**
	* \brief Efface la fenetre de log
	*	\fn void clearLog(int *line, WINDOW *win)
	* \param line Ligne acutelle du log
	* \param win Fenetre des logs
	*/
void clearLog(int *line, WINDOW *win){

	err ("<clearLog>", +1);
	clearArea(win, 1, 1, COLS_LOGS - 1, LINES_LOGS - 1);

	*line = 0;
	err ("</clearLog>", -1);
}

/**
	* \brief Ajoute une ligne à la fenetre de log
	*	\fn void addLog(char * message, int * line, WINDOW *win)
	* \param message Message à ajouter
	* \param line Ligne acutelle du log
	* \param win Fenetre des logs
	*/
void addLog(char * message, int * line, WINDOW *win){
	err ("<addLog>", +1);
	err(message, 0);

	wattron(win, COLOR_PAIR(GENERAL_COLOR));

	// On découpe le message en sous messages pour rentrer dans la zone de logs
	while(strlen(message) > COLS_LOGS-1) {

		wmove(win, (*line)+1, 1);
  	wprintw(win, "%.*s", COLS_LOGS-2, message);

		(*line)++;
    message+=COLS_LOGS-2;
  }
	wmove(win, (*line)+1, 1);
  wprintw(win, "%s", message);
	wrefresh(win);

	// Si on a plus de place pour clear la zone de logs
	if(*line >= LINES_LOGS - 3) clearLog(line, win);
	else (*line)++;
	err ("</addLog>", -1);

}

/**
	* \brief Affiche les objectifs du joueur dans la fenêtre de log
	*	\fn void displayObjectives(int *lineLog, WINDOW *win_logs)
	* \param lineLog Numéro de ligne de log
	* \param win_logs Fenêtre où afficher les objectifs
	*/
void displayObjectives(int *lineLog, WINDOW *win_logs){
	err ("<displayObjectives>", +1);
	(*lineLog)++;

	printLineCenter("-- OBJECTIFS -- ", COLS_LOGS, *lineLog, win_logs);

	*lineLog = 3;

	addLog("Vous venez d'apparaître au premier étage !", lineLog, win_logs);
	addLog("- Allez récupérer le papier contenant le mot", lineLog, win_logs);
	addLog("    de passe root du serveur info.", lineLog, win_logs);
	addLog("- Celui-ci se trouve tout en haut du batiment.", lineLog, win_logs);
	addLog("- Une fois récupéré, vous devez re-déscendre.", lineLog, win_logs);
	addLog("- Evitez de vous faire attraper par des", lineLog, win_logs);
	addLog("    étudiants.", lineLog, win_logs);
	addLog("- Appuyez sur `?` pour obtenir de l'aide.", lineLog, win_logs);

	*lineLog += 3;
	err ("</displayObjectives>", -1);

}

/**
	* \brief Crée une fenetre aux coordonnées indiquée, et de taille donnée
	*	\fn WINDOW *createWindow(int startX, int startY, int width, int height, char * label)
	* \param startX Décalage horizontal (en partant du point 0 à gauche)
	* \param startY Décalage vertical (en partant du point 0 en haut)
	* \param width Largeur de la fenêtre à créer
	* \param height Hauteur de la fenêtre à créer
	* \param label Nom de la fenêtre
	* \return Pointeur vers la fenetre créee
	*/
WINDOW *createWindow(int startX, int startY, int width, int height, char * label){

	err ("<createWindow>", +1);
	WINDOW *localWindow;

	err ("Création d'une nouvelle fenêtre", 0);

	localWindow = newwin(height, width, startY, startX);


	box(localWindow, 0, 0);
	wmove(localWindow,0,2);
	wprintw(localWindow, label);

	wrefresh(localWindow);

	err ("</createWindow>", -1);
	return localWindow;

}

/**
	* \brief Supprimer la fenêtre donnée
	*	\fn void deleteWindow(WINDOW *window)
	* \param window Nom de la fenêtre à supprimer
	*/
void deleteWindow(WINDOW *window){
	err ("<deleteWindow>", +1);

	int lines, cols;

	err ("Suppresion d'une fenêtre", 0);


	getmaxyx(window, lines, cols);

	clearArea(window, 0, 0, cols, lines);
	delwin(window);

	err ("</deleteWindow>", -1);

}


/**
	* \brief Affiche le contenu d'une cellule
	*	\fn void printCell(int pair, char cell, WINDOW *win)
	* \param pair Paire à choisir pour la couleur
	* \param cell Cellule à afficher (' ' ou 'c')
	* \param win Fenêtre où afficher la cellule
	*/
void printCell(int pair, char cell, WINDOW *win){
	//err ("<printCell>", +1);

	wattron(win, COLOR_PAIR(pair));
	switch (cell) {
		case 'c': waddch(win, ACS_CKBOARD); break;
		default: wprintw(win, "%c", cell); break;
	}
	//err ("</printCell>", -1);
}

/**
	* \brief Affiche l'étage de la map donnée en paramètre
	*	\fn void displayFloor(t_cell map[LINES][COLUMNS], t_character player, WINDOW *win, int visibleByGhost[LINES][COLUMNS])
	* \param map Carte à afficher
	* \param player Joueur
	* \param win Fenêtre où afficher la carte
	* \param visibleByGhost matrice pour savoir ce que les fantomes rendent visible.
	*/
void displayFloor(t_cell map[LINES][COLUMNS], t_character player, WINDOW *win, int visibleByGhost[LINES][COLUMNS]) {

	err ("<displayFloor/>", 0);
	int i, j;

	for (i = 0; i < LINES; i++) {
		wmove(win, i+1,1);
		for (j = 0; j < COLUMNS; j++) {
			if (map[i][j].isDiscovered || visibleByGhost[i][j] == 1) {
				switch (map[i][j].type) {

					case EMPTY: 	 printCell(GENERAL_COLOR,' ', win); break;
					case CORRIDOR:
						if (map[i][j].nbObject <= 0 || (map[i][j].obj[0].isDiscovered == FALSE && visibleByGhost[i][j] == 0))
							printCell(CORRIDOR_COLOR,'c', win);
						else {
							switch (map[i][j].obj[0].type) {
								case STAIRS_UP:   printCell(OBJECT_IN_CORRIDOR, '<', win); break;
								case STAIRS_DOWN: printCell(OBJECT_IN_CORRIDOR, '>', win); break;
								case FOOD:        printCell(OBJECT_IN_CORRIDOR, '%', win); break;
								case MED_KIT:     printCell(OBJECT_IN_CORRIDOR, '%', win); break;
								case TRAP:        printCell(OBJECT_IN_CORRIDOR, '^', win); break;
								case objNONE:     printCell(CORRIDOR_COLOR,' ', win); break;
								default: break;
							}
						}
						break;

					case DOORWAY:
						if (map[i][j].nbObject <= 0 || (map[i][j].obj[0].isDiscovered == FALSE && visibleByGhost[i][j] == 0)){
							switch (map[i][j].state) {
								case dNONE:  printCell(CORRIDOR_COLOR,'c', win); break;
								case dOPEN:  printCell(OPENED_DOOR,'c', win); break;
								case dCLOSE: printCell(GENERAL_COLOR,'c', win); break;
								default: printCell(GENERAL_COLOR,'?', win); break;
							}
						}
						else {
							switch (map[i][j].obj[0].type) {
								case STAIRS_UP:   printCell(OBJECT_IN_CORRIDOR, '<', win); break;
								case STAIRS_DOWN: printCell(OBJECT_IN_CORRIDOR, '>', win); break;
								case FOOD:        printCell(OBJECT_IN_CORRIDOR, '%', win); break;
								case MED_KIT:     printCell(MED_KIT_COLOR_C, '%', win); break;
								case TRAP:        printCell(OBJECT_IN_CORRIDOR, '^', win); break;
								case objNONE:     printCell(CORRIDOR_COLOR,' ', win); break;
								default: break;
							}
						}
						break;

					case ROOM:
						if (map[i][j].nbObject > 0 && 
							(map[i][j].obj[0].isDiscovered == TRUE || (visibleByGhost[i][j] == 1 && map[i][j].obj[0].type != TRAP))) 
							
							//if(map[i][j].obj[0].isDiscovered){
								switch (map[i][j].obj[0].type) {
									case STAIRS_UP:
										if(player.lvl >= NB_LVL -1){
											if(player.hasFoundObj) printCell(ROOM_COLOR,' ', win);
											else                   printCell(OBJECTS_COLOR,'O', win);
										}
										else
											printCell(OBJECTS_COLOR,'<', win);
									break;
									case STAIRS_DOWN: printCell(OBJECTS_COLOR, '>', win); break;
									case FOOD       : printCell(OBJECTS_COLOR, '%', win); break;
									case MED_KIT    : printCell(MED_KIT_COLOR, '%', win); break;
									case TRAP       : printCell(OBJECTS_COLOR, '^', win); break;
									case objNONE    : printCell(ROOM_COLOR,' ', win);     break;
								}
							//}
						else {
							printCell(ROOM_COLOR,' ', win);
						} break;


					case WALL: 		 printCell(WALL_COLOR,'c', win); break;
				}
			}
			else printCell(GENERAL_COLOR,' ', win); //EMPTY
		}

	}
	wrefresh(win);
	//err ("</displayFloor>", -1);

}

/**
	* \brief Affiche ou masque toute la map chargée
	*	\fn void setFloorCheat(t_cell map[LINES][COLUMNS], int etat)
	* \param map Carte à marquer comme explorée
	* \param etat Etat (TRUE pour afficher la carte, FALSE pour la masquer)
	*/
void setFloorCheat(t_cell map[LINES][COLUMNS], int etat) {

	err ("<setFloorCheat>", +1);
	int i, j;

	if(etat != TRUE && etat != FALSE){
		etat = TRUE;
	}
	
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			map[i][j].isDiscovered = etat;
			map[i][j].obj[0].isDiscovered = etat;
		}
	}

	err ("</setFloorCheat>", -1);
}

/**
	* \brief Affiche le joueur sur le jeu
	*	\fn void displayPlayer(t_character player, t_cell map[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line)
	* \param player Joueur
	* \param map Carte du jeu
	* \param win Fenêtre du jeu
	* \param logs Fenetre des logs
	* \param line Ligne acutelle du log
	*/
void displayPlayer(t_character player, t_cell map[LINES][COLUMNS], WINDOW *win, WINDOW *logs, int *line){

	err ("<displayPlayer>", +1);
	if(map[player.line][player.column].type == ROOM){
		wattron(win, COLOR_PAIR(PLAYER_COLOR));
	}
	else wattron(win, COLOR_PAIR(OBJECT_IN_CORRIDOR));

	wmove(win, (player.line)+1, (player.column)+1);
	wprintw(win, "@");
	wrefresh(win);

	if(map[player.line][player.column].nbObject != 0){

		switch (map[player.line][player.column].obj[0].type) {
			case STAIRS_UP:
				if(player.lvl < NB_LVL -1){
					addLog("Vous pouvez monter les escaliers avec :           > Entrée", line, logs);
				}
				else if(!player.hasFoundObj){
					addLog("Recuperez l'objet avec :                          > Entrée", line, logs);
				}
				break;
			case STAIRS_DOWN:
				if(player.lvl > 0){
					addLog("Vous pouvez déscendre les éscaliers avec :      > Entrée", line, logs); break;
				}
				else {
					if(player.hasFoundObj)
						addLog("Vous pouvez sortir du batiment ! :)", line, logs);
					else
						addLog("Vous ne pouvez pas encore sortir du batiment.", line, logs);
				}
			default: break;
		}

	}
	err ("</displayPlayer>", -1);

}

/**
	* \brief Affiche une barre
	*	\fn void printBar(int value, int max, WINDOW * win, int color)
	* \param value Taille de la barre remplie
	* \param max Taille de la taille totale
	* \param win Fenêtre où afficher la barre
	* \param color Couleur de la barre
	*/
void printBar(int value, int max, WINDOW * win, int color){
	err ("<printBar>", +1);

	int i;

	if(color) wattron(win, COLOR_PAIR(BAR_YELLOW));
	else      wattron(win, COLOR_PAIR(BAR_GREEN));

	for(i = 0 ; i < value && i < max ; i++){
		wprintw(win, " ");
	}

	wattron(win, COLOR_PAIR(BAR_RED));

	for(i = value ; i < max ; i++){
		wprintw(win, " ");
	}

	wrefresh(win);
	wattroff(win, COLOR_PAIR(BAR_RED));
	err ("</printBar>", -1);

}

/**
	* \brief Affiche l'inventaire du joueur
	*	\fn void printInventory(t_character player, WINDOW *win)
	* \param player Joueur à afficher son inventaire
	* \param win Fenêtre où afficher son inventaire
	* \param lineLog Ligne de logs
	*/
void printInventory(t_character player, WINDOW *win, int *lineLog){
	err ("<printBar>", +1);

	int i;
	char message[100];
	char nomItem[100];

	addLog("Voici le contenu de votre inventaire :", lineLog, win);
	addLog("", lineLog, win);

	for(i = 0 ; i < SIZE_INVENTORY ; i++){
		switch (player.inventory[i]) {
			case MED_KIT : sprintf(nomItem, "Kit de santé"); break;
			case FOOD    : sprintf(nomItem, "Nourriture");   break;
			case TRAP    : sprintf(nomItem, "Piége");        break;
			default      : sprintf(nomItem, "Vide");
		}
		sprintf(message, "Slot n°%i : %s", i, nomItem);
		addLog(message, lineLog, win);
	}

	addLog("", lineLog, win);

	err ("</printBar>", -1);

}

/**
	* \brief Affiche les statistiques du joueur
	*	\fn void displayStats(t_character player, WINDOW *win, int isPlayerInvicible)
	* \param player Joueur à afficher ses statistiques
	* \param win Fenêtre où afficher les statistiques
	* \param isPlayerInvicible Booléen représentant l'invincibilité du joueur
	*/
void displayStats(t_character player, WINDOW *win, int isPlayerInvicible){
	err ("<displayStats>", +1);

	clearArea(win, 1, 1, COLS_STATS - 1, LINES_STATS - 1);

	wmove(win, 1, 1);
	wrefresh(win);

	mvwprintw(win, 1, 1, "Etage     : %i / %i", player.lvl, NB_LVL -1);
	mvwprintw(win, 2, 1, "Vie       : ");
	printBar(player.hp, MAX_HP, win, isPlayerInvicible);

	mvwprintw(win, 3, 1, "Puissance : %i", player.pw);
	mvwprintw(win, 4, 1, "XP        : %i", player.xp);

	mvwprintw(win, 1, 30, "Nourriture   : ");
	printBar(player.food/10, MAX_FOOD/10, win, isPlayerInvicible);
	mvwprintw(win, 2, 30, "Déplacements : %i", player.nbMove);
	mvwprintw(win, 3, 30, "Joueur       : %s", player.name);

	wattron(win, COLOR_PAIR(GENERAL_COLOR));
	wmove(win, 4, 30);
	// Si le joueur est malade
	if(player.isSick){
		wprintw(win, "Empoisonné");
	}
	if(player.hasFoundObj){
		if(player.isSick) wprintw(win, " | ");
		wprintw(win, "Objet trouvé");
	}
	wattroff(win, COLOR_PAIR(GENERAL_COLOR));

	wrefresh(win);

	err ("</displayStats>", -1);
}

/**
	* \brief Afficher la fin du jeu
	*	\fn void displayEnd(t_character player, WINDOW *win)
	* \param player Joueur
	* \param win Fenêtre
	*/
void displayEnd(t_character player, WINDOW *win){
	err ("<displayEnd>", +1);

	int lines, columns;
	int yShift, xShift;
	getmaxyx(win, lines, columns);

	wattron(win, COLOR_PAIR(COLOR_TITLE));


	if(player.hp <= 0){
		printASCIICenter("include/game_over.txt", "Appuyez sur q pour quitter.", "Appuyez sur r pour recommencer une partie.", win);
	}
	else if(player.hasFoundObj == TRUE){
		printASCIICenter("include/well_done.txt", "Appuyez sur q pour quitter.", "Appuyez sur r pour recommencer une partie.", win);
	}

	wrefresh(win);
	err ("</displayEnd>", -1);
}
