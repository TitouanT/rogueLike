/**
	* \file
	* \brief Fonctions gérant les interactions utilisateurs / jeu
	* \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
	* \date 24 novembre 2016
	* \version 1.1
	*/
#include <ncurses.h>   // OK
#include <string.h>    // OK

#include "cell.h"      // OK
#include "character.h" // OK
#include "moves.h"     // OK
#include "display.h"   // OK
#include "mapConst.h"  // OK
#include "loadLvl.h"   // OK
#include "tools.h"     // OK

/** Nombre de messages d'erreurs différents */
#define NB_ERROR_MESSAGES 26

/** Taille maximum d'une commande de cheat */
#define MAX_CHEAT 45

/**
	*	\brief Définition d'une structure de messages d'erreur.
	*	\struct t_msg
	*/
typedef struct {char * msg;} t_msg;

/**
	* \var error_msg
	* \brief Liste des messages d'erreurs.
	*/
t_msg error_msg[NB_ERROR_MESSAGES] = {
																			"whatever",
																			"ha! ha! ha! ha!",
																			"believe it, baby",
																			"it's all good",
																			"not likely",
																			"you wish",
																			"say what ?",
																			"yeah, sure",
																			"probably",
																			"you are so hosed",
																			"if you want",
																			"uhhh... no",
																			"like i care",
																			"yep",
																			"uh huh",
																			"yo so funny",
																			"are you crazy ?",
																			"not even",
																			"oh yeah",
																			"true dat",
																			"fer sure",
																			"sorry",
																			"42",
																			"could be",
																			"r u mad?",
																			"STOP DOING THAT"
																		};



/**
	* \brief Message d'erreur si une touche appuyée n'existe pas
	* \fn void wrongKey(WINDOW * win, int *lineLog)
	* \param win Fenêtre de logs où afficher le message
	* \param lineLog Ligne d'écriture du message
	*/
void wrongKey(WINDOW * win, int *lineLog) {
	err ("<wrongKey>", +1);
	addLog(error_msg[randab(0, NB_ERROR_MESSAGES)].msg, lineLog, win);
	err ("</wrongKey>", -1);
}

/**
	* \brief Permet au joueur de manger la nourriture sur laquelle il se trouve
	*	\fn void eatFood(t_character *player, t_cell map[LINES][COLUMNS])
	* \param player Joueur
	* \param map Carte où se trouve le joueur
	*/
void eatFood(t_character *player, t_cell map[LINES][COLUMNS]){

	err ("<eatFood>", +1);
	int minFood = 20; /// minFood : Apport minimal de la nourriture à la faim du joueur
	int maxFood = 30; /// maxFood : Apport maximal de la nourriture à la faim du joueur

	map[player->line][player->column].obj[0].type = objNONE;
	map[player->line][player->column].nbObject = 0;

	player->food += randab(minFood, maxFood);
	if(player->food > MAX_FOOD) player->food = MAX_FOOD;

	// Possibilité de tomber malade en mangeant de la nourriture avariée
	if(didItHappen(CHANCE_SICK)){
		player->isSick = TRUE;
	}
	err ("</eatFood>", -1);

}


/**
	* \brief Drop un item par terre
	* \fn void dropItem(t_character *player, t_cell map[LINES][COLUMNS], WINDOW *win_logs, int *lineLog)
	* \param player Joueur
	* \param map Carte où se trouve le joueur
	* \param win_logs Fenêtre de logs
	* \param lineLog Ligne d'écriture des logs
	*/
void dropItem(t_character *player, t_cell map[LINES][COLUMNS], WINDOW *win_logs, int *lineLog){
	err ("<dropItem>", +1);

	int key;
	int i;

	printInventory(*player, win_logs, lineLog);
	addLog("", lineLog, win_logs);
	addLog("Quel item voulez vous jeter ?", lineLog, win_logs);
	addLog(" (donnez son numéro)  > ", lineLog, win_logs);

	key = getch();
	waddch(win_logs, key);
	i = key - '0';

	wrefresh(win_logs);

	if(isBetween(i, 0, SIZE_INVENTORY-1)){

		if(player->inventory[i] != objNONE){
			if(map[player->line][player->column].nbObject == 0){
				map[player->line][player->column].obj[0].type = player->inventory[i];
				map[player->line][player->column].nbObject = 1;
				map[player->line][player->column].obj[0].isDiscovered = TRUE;
				player->inventory[i] = objNONE;
				addLog("L'objet a bien été posé au sol.", lineLog, win_logs);
			}
			else {
				addLog("Il y a déjà un objet à cet endroit.", lineLog, win_logs);
			}
		}
		else {
			addLog("Votre inventaire est vide à cet endroit !", lineLog, win_logs);
		}

	}
	else {
		addLog("Ce slot n'existe pas !", lineLog, win_logs);
	}
	err ("</dropItem>", -1);
}

/**
	* \brief Récupère l'objet sur lequel le joueur se trouve
	* \fn void grabItem(t_character *player, t_cell map[LINES][COLUMNS], WINDOW *win_logs, int *lineLog)
	* \param player Joueur
	* \param map Carte où se trouve le joueur
	* \param win_logs Fenêtre de logs
	* \param lineLog Ligne d'écriture des logs
	*/
void grabItem(t_character *player, t_cell map[LINES][COLUMNS], WINDOW *win_logs, int *lineLog){
	err ("<grabItem>", +1);

	int i;

    //err((char *)map[player->line][player->column].obj[map[player->line][player->column].nbObject].type);

	if(map[player->line][player->column].nbObject > 0){

		err("Le joueur se trouve sur une case avec au minimum un objet", 0);

		for(i = 0 ; i < SIZE_INVENTORY && player->inventory[i] != objNONE; i++);

		if(i < SIZE_INVENTORY && player->inventory[i] == objNONE){

			switch (map[player->line][player->column].obj[0].type) {
				case MED_KIT:
				case FOOD   :
				case TRAP   :
					player->inventory[i] = map[player->line][player->column].obj[0].type;
					map[player->line][player->column].obj[0].type = objNONE;
					map[player->line][player->column].nbObject = 0;
					break;
				default     : addLog("Vous ne pouvez pas récupérer cet objet !", lineLog, win_logs);
			}

		}
		else {
			addLog("Votre inventaire est plein !", lineLog, win_logs);
			addLog("  > Lachez des objets avec la touche 'd'", lineLog, win_logs);
		}

		addLog("", lineLog, win_logs);
		printInventory(*player, win_logs, lineLog);
	}
	else {
		addLog("Il n'y a aucun item à récupérer !", lineLog, win_logs);
	}

	err ("</grabItem>", -1);
	
}

/**
	* \brief Demande à l'utilisateur de saisir un code de triche
	* \fn void cheat(WINDOW *win_logs, WINDOW *win_game, t_cell map[LINES][COLUMNS], t_character *player, int *isPlayerInvicible)
	* \param win_logs Fenêtre de logs
	* \param win_game Fenêtre de jeu
	* \param map Carte
	* \param player Joueur
	* \param isPlayerInvicible Booléen qui réprésente l'invincibilité du joueur
	*/
void cheat(WINDOW *win_logs, WINDOW *win_game, t_cell map[LINES][COLUMNS], t_character *player, int *isPlayerInvicible){
	err ("<cheat>", +1);

	char cheatSTR[MAX_CHEAT];
	int lineLog = 0;

	// Faudrait changer ça, mais là n'est pas la priorité
	init_pair(100, COLOR_WHITE, COLOR_BLACK);
	init_pair(101, COLOR_RED, COLOR_BLACK);

	echo();

	wattron(win_logs, COLOR_PAIR(101));
	mvwprintw(win_logs, LINES_LOGS-3, 1, "Indiquez votre code de triche :");
	wmove(win_logs, LINES_LOGS-2, 2);

	wattron(win_logs, COLOR_PAIR(100));
	wrefresh(win_logs);
	wgetnstr(win_logs, cheatSTR, MAX_CHEAT);

	wattroff(win_logs, COLOR_PAIR(100));
	noecho();

	if(strcmp(cheatSTR, "lumos") == 0){
		setFloorCheat(map);
	}
	else if(strcmp(cheatSTR, "food") == 0){
		player->food = MAX_FOOD;
	}
	else if(strcmp(cheatSTR, "food++") == 0){
		if(player->food < MAX_FOOD) player->food += 10;
	}
	else if(strcmp(cheatSTR, "food--") == 0){
		if(player->food >= 10) player->food -= 10;
	}
	else if(strcmp(cheatSTR, "I WANT TO EAT") == 0){
		player->food = 0;
	}
	else if(strcmp(cheatSTR, "heal") == 0){
		player->hp = MAX_HP;
		player->isSick = FALSE;
	}
	else if(strcmp(cheatSTR, "invincible") == 0){
		*isPlayerInvicible = TRUE;
	}
	else if(strcmp(cheatSTR, "damage") == 0){
		player->hp -= 1;
	}
	else if(strcmp(cheatSTR, "suicide fail") == 0){
		player->hp = 1;
	}
	else if(strcmp(cheatSTR, "kill") == 0){
		player->hp = 0;
	}
	else if(strcmp(cheatSTR, "sick") == 0){
		player->isSick = TRUE;
	}
	else if(strcmp(cheatSTR, "up") == 0){
		changeLvl(map,player, 1);
	}
	else if(strcmp(cheatSTR, "down") == 0){
		changeLvl(map,player, -1);
	}
	else if(strcmp(cheatSTR, "exit") == 0){

	}
	else {
		clearLog(&lineLog, win_logs);
		lineLog++;

		printLineCenter("-- AIDE POUR LES TRICHEURS -- ", COLS_LOGS, lineLog, win_logs);

		lineLog += 2;

		addLog("help          : Affiche cette liste d'aide", &lineLog, win_logs);
		addLog("lumos         : Affiche la map au complet", &lineLog, win_logs);
		addLog("food          : Met 100% de la nourriture", &lineLog, win_logs);
		addLog("food++        : Ajoute 1pt de nourriture", &lineLog, win_logs);
		addLog("food--        : Enlève 1pt de nourriture", &lineLog, win_logs);
		addLog("I WANT TO EAT : Met 0% de la nourriture", &lineLog, win_logs);
		addLog("heal          : Met 100% de la vie", &lineLog, win_logs);
		addLog("invincible    : Rend le joueur invincible", &lineLog, win_logs);
		addLog("damage        : Enlève 1pt de vie", &lineLog, win_logs);
		addLog("suicide fail  : Met la vie à 1", &lineLog, win_logs);
		addLog("kill          : Tue le joueur", &lineLog, win_logs);
		addLog("sick          : Rend le joueur malade", &lineLog, win_logs);
		addLog("up            : Monte le joueur d'un étage", &lineLog, win_logs);
		addLog("down          : Descend le joueur d'un étage", &lineLog, win_logs);
		addLog("exit          : Sortir de ce menu", &lineLog, win_logs);

		cheat(win_logs, win_game, map, player, isPlayerInvicible);

		clearLog(&lineLog, win_logs);
	}
	err ("</cheat>", -1);
}

/**
	* \brief Affiche un descriptif des commandes
	* \fn void help(WINDOW *win_logs, int *lineLog)
	* \param win_logs Fenêtre de logs
	* \param lineLog Ligne d'écriture des logs
	*/
void help(WINDOW *win_logs, int *lineLog){
	err ("<help>", +1);
	
	(*lineLog)++;

	printLineCenter("-- AIDE POUR LES NOUVEAUX -- ", COLS_LOGS, *lineLog, win_logs);

	*lineLog += 2;

	addLog("?         : Affiche cette liste d'aide", lineLog, win_logs);
	addLog("flèches   : Permet de se déplacer", lineLog, win_logs);
	addLog("o         : Ouvrir une porte", lineLog, win_logs);
	addLog("f         : Fermer une porte", lineLog, win_logs);
	addLog("i         : Voir l'inventaire", lineLog, win_logs);
	addLog("g         : Prendre un objet", lineLog, win_logs);
	addLog("d         : Poser un objet de l'inventaire", lineLog, win_logs);
	addLog("s         : Sauvegarder la partie", lineLog, win_logs);
	addLog("q         : Quitter la partie (sans sauvegarde)", lineLog, win_logs);
	addLog("entrée    : Interagir", lineLog, win_logs);

	*lineLog += 3;
	err ("</help>", -1);
}

/**
	* \brief Teste si la porte est valide
	*	\fn int bIsValidDoor(t_cell map[LINES][COLUMNS], t_pos position, t_monster monsters[NB_MONSTER_MAX], int nbMonster, int playerLvl)
	* \param map Carte où se trouve le joueur
	* \param position Position de la porte à verifier
	* \param monsters Monstres présents dans le jeu
	* \param nbMonster Nombre de monstres dans le jeu
	* \param playerLvl Niveau du joueur (son étage)
	* \return TRUE si la porte est valide
	* \return FALSE sinon
	*/
int bIsValidDoor(t_cell map[LINES][COLUMNS], t_pos position, t_monster monsters[NB_MONSTER_MAX], int nbMonster, int playerLvl){
	err ("<bIsValidDoor>", +1);

	int i;
	// On vérifie d'abord qu'un monstre ne se trouve pas à la position de la porte
	for(i = 0 ; i < nbMonster ; i++){
		if(monsters[i].lvl == playerLvl && monsters[i].line == position.line && monsters[i].col == position.column && monsters[i].hp > 0){
			return FALSE;
		}
	}

	err ("</bIsValidDoor>", -1);
	return (
		position.line >= 0 &&
		position.column >= 0 &&
		position.line < LINES &&
		position.column < COLUMNS &&
		map[position.line][position.column].type == DOORWAY &&
		map[position.line][position.column].nbObject <= 0
	);
}

/**
	* \brief Traite l'ouverture et la fermeture d'une porte
	*	\fn void traiterPorte(t_cell map[LINES][COLUMNS], t_character *player, int key, WINDOW * win, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster)
	* \param map Carte où se trouve le joueur
	* \param player Joueur sur la carte
	* \param key Touche appuyée par l'utilisateur
	* \param win Fenêtre de logs
	* \param lineLog Ligne où afficher les logs
	* \param monsters Monstres présents dans le jeu
	* \param nbMonster Nombre de monstres dans le jeu
	*/
void traiterPorte(t_cell map[LINES][COLUMNS], t_character *player, int key, WINDOW * win, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster){
	err ("<traiterPorte>", +1);

	int direction;
	t_pos doorPos = {player->line, player->column};

	addLog("Quelle porte voulez vous ouvrir ?", lineLog, win);
	addLog("  (flèche haute, basse, gauche, droite)", lineLog, win);

	direction = getch();

  	switch (direction) {

		case 'k':
		case KEY_UP    : (doorPos.line)--;   break;
		
		case 'j':
		case KEY_DOWN  : (doorPos.line)++;   break;

		case 'h':
		case KEY_LEFT  : (doorPos.column)--; break;

		case 'l':
		case KEY_RIGHT : (doorPos.column)++; break;
		
		default: wrongKey(win, lineLog);
	}

	if(key == 'o'){

		if(bIsValidDoor(map, doorPos, monsters, nbMonster, player->lvl) && map[doorPos.line][doorPos.column].state == dCLOSE){
		// Ajoute une probabilité de 30% de ne pas réussir à ouvrir la porte
		if(didItHappen(30)) {
			addLog("Vous venez d'enfoncer cette porte.", lineLog, win);
			addLog("Recommencez pour l'ouvrir entièrement !", lineLog, win);
		}
		else map[doorPos.line][doorPos.column].state = dOPEN;

		(player->nbMove)++;
	}
	else addLog("Ouverture impossible.", lineLog, win);

	}
	else if(key == 'c'){

		if(bIsValidDoor(map, doorPos, monsters, nbMonster, player->lvl) && map[doorPos.line][doorPos.column].state == dOPEN){
			map[doorPos.line][doorPos.column].state = dCLOSE;
		}
		else addLog("Fermeture impossible.", lineLog, win);
	}

	err ("</traiterPorte>", -1);

}

/**
	* \brief Traite l'appui sur la touche entrée
	*	\fn int traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win, int *lineLog)
	* \param map Carte où se trouve le joueur
	* \param player Joueur sur la carte
	* \param win Fenêtre de logs
	* \param lineLog Ligne où afficher les logs
	* \return FALSE si le joueur souhaite sortir du chateau
	* \return TRUE sinon
	*/
int traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win, int *lineLog){
	err ("<traiterPorte>", +1);


	if(map[player->line][player->column].nbObject > 0){

		switch (map[player->line][player->column].obj[0].type) {

			case STAIRS_UP:
				if(player->lvl < NB_LVL - 1){
					changeLvl(map,player,1);
				}
				else {
					player->hasFoundObj = TRUE;
				}
				break;

			case STAIRS_DOWN:
				if(player->lvl > 0){
					changeLvl(map,player, -1);
				}
				else {
					if(player->hasFoundObj == FALSE) {
						addLog("Vous ne pouvez pas sortir du chateau sans avoir trouvé l'objet !", lineLog, win);
					}
					else {
						err ("</traiterPorte>", -1);
						return FALSE;
					}
				}
				break;
				
			case FOOD:
				if(player->food >= MAX_FOOD){
					addLog("Vous n'avez plus faim !", lineLog, win);
				}
				else {
					eatFood(player, map);
				}
				break;
				
			case MED_KIT:
				if(player->hp < MAX_HP || player->isSick){
					player->hp = min(MAX_HP, player->hp + randab(5, 10));
					player->isSick = FALSE;
					map[player->line][player->column].obj[0].type = objNONE;
					map[player->line][player->column].nbObject = 0;
				}
				break;
			
			default: break;
		}
	}
	else {
		addLog("Commande invalide.", lineLog, win);
	}
	err ("</traiterPorte>", -1);
	return TRUE;
}

/**
	* \brief Demande confirmation à l'utilisateur avant de quitter le jeu
	*	\fn int askConfirmationToQuit(WINDOW * win, int *lineLog)
	* \param win Fenêtre de logs
	* \param lineLog Ligne où afficher les logs
	* \return TRUE si l'utilisateur souhaite quitter le jeu
	* \return FALSE sinon
	*/
int askConfirmationToQuit(WINDOW * win, int *lineLog) {
	err ("<askConfirmationToQuit>", +1);

	int key;

	addLog("Etes-vous sur de vouloir quitter sans sauvegarder ? (y/n)", lineLog, win);

	key = getch();

	switch (key) {
		case 'y': {
			err ("</askConfirmationToQuit>", -1);
			return TRUE;
		}
		case 'n': break;

		default: wrongKey(win, lineLog);
	}
	err ("</askConfirmationToQuit>", -1);
	return FALSE;
}

/**
	* \brief Fonction principale d'intéraction avec l'utilisateur
	* \fn int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW * win_game, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster, int *isPlayerInvicible, int visibleByGhost[LINES][COLUMNS])
	* \param key Touche que l'utilisateur a appuyé
	* \param map Carte où se trouve le joueur
	* \param player Joueur
	* \param win_logs Fenêtre de logs où afficher le message
	* \param win_game Fenêtre de jeu
	* \param lineLog Ligne d'écriture du message
	* \param monsters la lise des monstres
	* \param nbMonster le nombre de monstre
	* \param isPlayerInvicible Booléen qui représente si le joueur est invincible
	* \param visibleByGhost Matrice pour savoir ce que les fantomes rendent visible
	* \return FALSE si l'utilisateur à demandé de quitter la partie
	* \return TRUE sinon
	*/
int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW * win_game, int *lineLog, t_monster monsters[NB_MONSTER_MAX], int nbMonster, int *isPlayerInvicible,  int visibleByGhost[LINES][COLUMNS]){
	err ("<handleInteraction>", +1);
	switch (key) {
		case 'k': case KEY_UP:
			move_perso(UP,    map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost);
			break;
			
		case 'j': case KEY_DOWN:
			move_perso(DOWN,  map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost);
			break;
			
		case 'h': case KEY_LEFT:
			move_perso(LEFT,  map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost);
			break;
			
		case 'l': case KEY_RIGHT:
			move_perso(RIGHT, map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost);
			break;
			

		case 'y': move_perso(UP_LEFT,    map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost); break;
		case 'u': move_perso(UP_RIGHT,   map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost); break;
		case 'b': move_perso(DOWN_LEFT,  map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost); break;
		case 'n': move_perso(DOWN_RIGHT, map, player, win_logs, lineLog, monsters, nbMonster, win_game, visibleByGhost); break;


		case '\n': err ("</handleInteraction>", -1); return (traiterEntree(map, player, win_logs, lineLog));
		case 'o' : traiterPorte (map, player, key, win_logs, lineLog, monsters, nbMonster);   break;
		case 'c' : traiterPorte (map, player, key, win_logs, lineLog, monsters, nbMonster);   break;
		case 's' :
			addLog("Sauvegarde en cours", lineLog, win_logs);
			saveGame(map, player, monsters, nbMonster);
			addLog("Partie sauvegardée", lineLog, win_logs);
			break;
			
		case 'q' : err ("</handleInteraction>", -1); return FALSE;
		case 'Q' : err ("</handleInteraction>", -1); return !askConfirmationToQuit(win_logs, lineLog);

		case '_' : cheat(win_logs, win_game, map, player, isPlayerInvicible); break;

		case '?' : help(win_logs, lineLog); break;

		case 'i' : printInventory(*player, win_logs, lineLog); break;
		case 'g' : grabItem(player, map, win_logs, lineLog); break;
		case 'd' : dropItem(player, map, win_logs, lineLog); break;

		default: wrongKey(win_logs, lineLog);
	}
	err ("</handleInteraction>", -1);
	return TRUE;
}
