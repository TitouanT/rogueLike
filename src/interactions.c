/**
	* \file
	* \brief Fonctions gérant les interactions utilisateurs / jeu
	* \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
	* \date 24 novembre 2016
	* \version 1.1
	*/
#include <ncurses.h>
#include "global.h"
#include "filePos.h"
#include "tools.h"

/** Nombre de messages d'erreurs différents */
#define NB_ERROR_MESSAGES 26

/**
	*	\brief Définition d'une structure de messages d'erreur.
	*	\struct t_msg
	*/
typedef struct {char * msg;} t_msg;


void traiterPorte(t_cell map[LINES][COLUMNS],  t_character *player, int key, WINDOW * win, int *lineLog);
int traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win, int *lineLog);
int askConfirmationToQuit(WINDOW * win, int *lineLog);


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
	*	\fn void wrongKey(WINDOW * win, int *lineLog)
	* \param win Fenêtre de logs où afficher le message
	* \param lineLog Ligne d'écriture du message
	*/
void wrongKey(WINDOW * win, int *lineLog) {
	addLog(error_msg[randab(0, NB_ERROR_MESSAGES)].msg, lineLog, win);
}

/**
	* \brief Teste si le joueur peut se déplacer (si il a suffisament de nourriture)
	* Le joueur a 25% de chance de pouvoir se déplacer même en ayant plus de nourriture
	*	\fn int canPlayerMove(t_character *player)
	* \param player Joueur
	* \return TRUE si le joueur peut se déplacer
	*/
int canPlayerMove(t_character *player){
	if(player->food == 0){
		return didItHappen(25);
	}
	return TRUE;
}

/**
	* \brief Augmente la faim du joueur de façon aléatoire
	* Le joueur a 30% de chance de perdre de la nourriture à chaque mouvement
	*	\fn void augmenterFaim(t_character *player)
	* \param player Joueur
	*/
void augmenterFaim(t_character *player){
	if(canPlayerMove(player)){
		if(player->food > 0 && didItHappen(30)) (player->food)--;
	}
}

/**
	* \brief Permet au joueur de manger la nourriture sur laquelle il se trouve
	*	\fn void eatFood(t_character *player, t_cell map[LINES][COLUMNS])
	* \param player Joueur
	* \param map Carte où se trouve le joueur
	*/
void eatFood(t_character *player, t_cell map[LINES][COLUMNS]){

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

}

/**
	* \brief Fonction principale d'intéraction avec l'utilisateur
	* \fn int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW * win_game, int *lineLog)
	* \param key Touche que l'utilisateur a appuyé
	* \param map Carte où se trouve le joueur
	* \param player Joueur
	* \param win_logs Fenêtre de logs où afficher le message
	* \param win_game Fenêtre de jeu
	* \param lineLog Ligne d'écriture du message
	* \return FALSE si l'utilisateur à demandé de quitter la partie
	* \return TRUE sinon
	*/
int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, WINDOW * win_game, int *lineLog){

	err("*** debut handleInteraction ***");
	switch (key) {
		case 'k': case KEY_UP:    move_perso(UP,    map, player, win_logs, lineLog);  break;
		case 'j': case KEY_DOWN:  move_perso(DOWN,  map, player, win_logs, lineLog);  break;
		case 'h': case KEY_LEFT:  move_perso(LEFT,  map, player, win_logs, lineLog);  break;
		case 'l': case KEY_RIGHT: move_perso(RIGHT, map, player, win_logs, lineLog);  break;

		case 'y': move_perso(UP_LEFT, map, player, win_logs, lineLog);    break;
		case 'u': move_perso(UP_RIGHT, map, player, win_logs, lineLog);   break;
		case 'b': move_perso(DOWN_LEFT, map, player, win_logs, lineLog);  break;
		case 'n': move_perso(DOWN_RIGHT, map, player, win_logs, lineLog); break;


    case '\n': return (traiterEntree(map, player, win_logs, lineLog));
    case 'o' : traiterPorte (map, player, key, win_logs, lineLog);   break;
    case 'c' : traiterPorte (map, player, key, win_logs, lineLog);   break;
		case 's' : err("****** Sauvegarde en cours ******"); saveGame(map, player); addLog("Partie sauvegardée", lineLog, win_logs); err("****** Partie sauvegardée ******"); break;//
    case 'q' : return FALSE;
    case 'Q' : return !askConfirmationToQuit(win_logs, lineLog);

		case '_' : cheat(win_logs, win_game, map, player); break;

		case 'i' : printInventory(*player, win_logs, lineLog); break;
		case 'g' : grabItem(player, map, win_logs, lineLog); break;
		case 'd' : dropItem(player, map, win_logs, lineLog); break;

		default: wrongKey(win_logs, lineLog);
	}

  markDiscover(map, *player);

  err("*** fin handleInteraction ***");
  return TRUE;

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

	int i;
	err("*** on entre dans la fonction de grab d'un item ***");

//err((char *)map[player->line][player->column].obj[map[player->line][player->column].nbObject].type);

	if(map[player->line][player->column].nbObject > 0){

		err(" > Le joueur se trouve sur une case avec au minimum un objet");

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


}


/**
	* \brief Teste si la porte est valide
	*	\fn int bIsValidDoor(t_cell map[LINES][COLUMNS], t_pos position)
	* \param map Carte où se trouve le joueur
	* \param position Position de la porte à verifier
	* \return TRUE si la porte est valide
	* \return FALSE sinon
	*/
int bIsValidDoor(t_cell map[LINES][COLUMNS], t_pos position){
  return (position.line >= 0 && position.column >= 0 && position.line < LINES && position.column < COLUMNS && map[position.line][position.column].type == DOORWAY);
}



/**
	* \brief Traite l'ouverture et la fermeture d'une porte
	*	\fn void traiterPorte(t_cell map[LINES][COLUMNS], t_character *player, int key, WINDOW * win, int *lineLog)
	* \param map Carte où se trouve le joueur
	* \param player Joueur sur la carte
	* \param key Touche appuyée par l'utilisateur
	* \param win Fenêtre de logs
	* \param lineLog Ligne où afficher les logs
	*/
void traiterPorte(t_cell map[LINES][COLUMNS], t_character *player, int key, WINDOW * win, int *lineLog){

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

    if(bIsValidDoor(map, doorPos) && map[doorPos.line][doorPos.column].state == dCLOSE){
			// Ajoute une probabilité de ne pas réussir à ouvrir la porte
      if(randab(0,3) == 0) {
        addLog("Vous venez d'enfoncer cette porte.", lineLog, win);
        addLog("Recommencez pour l'ouvrir entièrement !", lineLog, win);
      }
      else map[doorPos.line][doorPos.column].state = dOPEN;

      (player->nbMove)++;
    }
    else addLog("Ouverture impossible.", lineLog, win);

  }
  else if(key == 'c'){

    if(bIsValidDoor(map, doorPos) && map[doorPos.line][doorPos.column].state == dOPEN){
      map[doorPos.line][doorPos.column].state = dCLOSE;
    }
    else addLog("Fermeture impossible.", lineLog, win);
  }


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
					else return FALSE;
        }
        break;
			case FOOD:
				if(player->food >= MAX_FOOD){
					addLog("Vous n'avez plus faim !", lineLog, win);
				}
				else {
					eatFood(player, map);
				} break;
			case MED_KIT:
				if(player->hp < MAX_HP || player->isSick){
					player->hp = min(MAX_HP, player->hp + randab(5, 10));
					player->isSick = FALSE;
					map[player->line][player->column].obj[0].type = objNONE;
					map[player->line][player->column].nbObject = 0;
				} break;
      default: break ;

    }
  }
  else {
    addLog("Commande invalide.", lineLog, win);
  }

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

	int key;

	addLog("Etes-vous sur de vouloir quitter sans sauvegarder ? (y/n)", lineLog, win);

	key = getch();

	switch (key) {
		case 'y': return TRUE;
		case 'n': break;

		default: wrongKey(win, lineLog);
	}

 /*	addLog("Voulez-vous sauvegarder et quitter ? (y/n)", lineLog, win);

	key = getch();

	switch (key) {
		case 'y': saveGame(map, player); addLog("Partie Sauvegardée", lineLog, win); return TRUE;
		case 'n': return FALSE;
		default: wrongKey(win, lineLog);
	}
	*/

	return FALSE;
}


/**
	* \brief Evanouissement du joueur
	* Selectionne un endroit aléatoire de la carte, et le dé-mémorise
	*	\fn void passOut(t_cell map[LINES][COLUMNS])
	* \param map Carte
	*/
void passOut(t_cell map[LINES][COLUMNS]){

	int line, column, height, width, maxHeight, maxWidth;
	int i, j;

	int aMinLen = 10, aMaxHeight = 15, aMaxWidth = 20;

	line   = randab(1, LINES - aMinLen -1);
	column = randab(1, COLUMNS - aMinLen -1);

	maxHeight = min(LINES - line - 1, aMaxHeight);
	height    = randab(aMinLen, maxHeight +1);

	maxWidth = min(COLUMNS - column - 1, aMaxWidth);
	width    = randab(aMinLen, maxWidth +1);

	for(i = line ; i < maxHeight + line ; i++){
		for(j = column ; j < maxWidth + column ; j++){
			map[i][j].isDiscovered = FALSE;
		}
	}

}


/**
	* \brief Demande à l'utilisateur de saisir un code de triche
	*	\fn void cheat(WINDOW *win_logs, WINDOW *win_game, t_cell map[LINES][COLUMNS], t_character *player)
	* \param win_logs Fenêtre de logs
	* \param win_game Fenêtre de jeu
	* \param map Carte
	* \param player Joueur
	*/
void cheat(WINDOW *win_logs, WINDOW *win_game, t_cell map[LINES][COLUMNS], t_character *player){

	char cheatSTR[COLS_LOGS];

	int lineLog = 0;

	echo();

	mvwprintw(win_logs, LINES_LOGS-3, 1, "Indiquez votre code de triche :");
	move(LINES_LOGS-2, COLS_GAME+2);

	wrefresh(win_logs);
	getstr(cheatSTR);
	noecho();

	if(strcmp(cheatSTR, "ToWinICheat") == 0){
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


		addLog("        -- AIDE POUR LES TRICHEURS -- ", &lineLog, win_logs);

		addLog("", &lineLog, win_logs);

		addLog("?             : Affiche cette liste d'aide", &lineLog, win_logs);
		addLog("help          : Affiche cette liste d'aide", &lineLog, win_logs);
		addLog("ToWinICheat   : Affiche la map au complet", &lineLog, win_logs);
		addLog("food          : Met 100% de la nourriture", &lineLog, win_logs);
		addLog("food++        : Ajoute 1pt de nourriture", &lineLog, win_logs);
		addLog("food--        : Enlève 1pt de nourriture", &lineLog, win_logs);
		addLog("I WANT TO EAT : Met 0% de la nourriture", &lineLog, win_logs);
		addLog("heal          : Met 100% de la vie", &lineLog, win_logs);
		addLog("damage        : Enlève 1pt de vie", &lineLog, win_logs);
		addLog("suicide fail  : Met la vie à 1", &lineLog, win_logs);
		addLog("kill          : Tue le joueur", &lineLog, win_logs);
		addLog("sick          : Rend le joueur malade", &lineLog, win_logs);
		addLog("up            : Monte le joueur d'un étage", &lineLog, win_logs);
		addLog("down          : Descend le joueur d'un étage", &lineLog, win_logs);
		addLog("exit          : Sortir de ce menu", &lineLog, win_logs);

		cheat(win_logs, win_game, map, player);

		clearLog(&lineLog, win_logs);

	}


}

void fallTrap(t_cell map[LINES][COLUMNS], t_character *perso, WINDOW *win_logs, int *lineLog, t_dir direction){
	int trapType=randab(0, 3);
	int lostLvl, lostHp, glisser;

	switch(trapType){
		case 0 : lostLvl=randab(0,perso->lvl+1)*-1; changeLvl(map, *&perso, lostLvl); addLog("Vous êtes tombé dans un trou", lineLog, win_logs); break;
		case 1 : lostHp=randab(0,5); (perso->hp)= (perso->hp)-lostHp; addLog("Attention, un L1 vous a jeté une carte, vous vous êtes écorché", lineLog, win_logs); break;
		case 2 : glisser=randab(2,8); for(int i=0; i<glisser;i++) move_perso(direction, map, perso, win_logs, lineLog); addLog("Regardez où vous mettez vos pieds, la femme de ménage à lustrer le sol", lineLog, win_logs);
	}
}
