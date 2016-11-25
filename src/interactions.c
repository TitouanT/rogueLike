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
#include "random.h"

/** Nombre de messages d'erreurs différents */
#define NB_ERROR_MESSAGES 26

/**
	*	\brief Définition d'une structure de messages d'erreur.
	*	\struct t_msg
	*/
typedef struct {char * msg;} t_msg;


void traiterPorte(t_cell map[LINES][COLUMNS],  t_character *player, int key, WINDOW * win, int *lineLog);
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win, int *lineLog);
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
	*	\fn int canPlayerMove(t_character *player)
	* \param player Joueur
	* \return FALSE si le joueur ne peut pas se déplacer
	*/
int canPlayerMove(t_character *player){
	return player->food != 0;
}

/**
	* \brief Augmente la faim du joueur de façon aléatoire
	*	\fn void augmenterFaim(t_character *player)
	* \param player Joueur
	*/
void augmenterFaim(t_character *player){
	if(canPlayerMove(player)){
		if(randab(0,3) == 0) (player->food)--;
	}
}


/**
	* \brief Fonction principale d'intéraction avec l'utilisateur
	*	\fn int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, int *lineLog)
	* \param key Touche que l'utilisateur a appuyé
	* \param map Carte où se trouve le joueur
	* \param player Joueur
	* \param win_logs Fenêtre de logs où afficher le message
	* \param lineLog Ligne d'écriture du message
	* \return FALSE si l'utilisateur à demandé de quitter la partie
	* \return TRUE sinon
	*/
int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, int *lineLog){

	augmenterFaim(player);

  switch (key) {
		case 'k': case KEY_UP:    move_perso(UP,    map, player);  break;
		case 'j': case KEY_DOWN:  move_perso(DOWN,  map, player);  break;
		case 'h': case KEY_LEFT:  move_perso(LEFT,  map, player);  break;
		case 'l': case KEY_RIGHT: move_perso(RIGHT, map, player);  break;

		case 'y': move_perso(UP_LEFT, map, player);  break;
		case 'u': move_perso(UP_RIGHT, map, player);  break;
		case 'b': move_perso(DOWN_LEFT, map, player);  break;
		case 'n': move_perso(DOWN_RIGHT, map, player);  break;

    case '\n':      traiterEntree(map, player,  win_logs,     lineLog); break;
    case 'o' :      traiterPorte (map, player, key, win_logs, lineLog); break;
    case 'c' :      traiterPorte (map, player, key, win_logs, lineLog); break;

    case 'q' : return FALSE;
    case 'Q' : return !askConfirmationToQuit(win_logs, lineLog);

		//case '_' : handleCheat(map, player, win_logs, lineLog); break;

		//case 'n' : randomFloor(map, 5); move2spawn(map, player, STAIRS_DOWN); break; // a mettre dans cheat
		case 'N' : randomFloor(map, 5); move2spawn(map, player, STAIRS_UP); break; // a mettre dans cheat

		default: wrongKey(win_logs, lineLog);
	}

  markDiscover(map, *player);

  return TRUE;

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
	*	\fn void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win, int *lineLog)
	* \param map Carte où se trouve le joueur
	* \param player Joueur sur la carte
	* \param win Fenêtre de logs
	* \param lineLog Ligne où afficher les logs
	*/
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win, int *lineLog){


  if(map[player->line][player->column].nbObject > 0){

    switch (map[player->line][player->column].obj[0]) {

      case STAIRS_UP:
        if(player->lvl < NB_LVL - 1){
          changeLvl(map,player,1);
        }
        else {
          addLog("Vous êtes déjà au niveau le plus haut !", lineLog, win);
        }
      break;

      case STAIRS_DOWN:
        if(player->lvl > 0){
          changeLvl(map,player, -1);
	}
        else {
          addLog("Vous êtes déjà en bas !", lineLog, win);
        }
        break;

      default: break ;

    }
  }
  else {
    addLog("Commande invalide.", lineLog, win);
  }

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

	addLog("Etes-vous sur de vouloir quitter ? (y/n)", lineLog, win);

	key = getch();

	switch (key) {
		case 'y': return TRUE;
		case 'n': return FALSE;

		default: wrongKey(win, lineLog);
	}
	return FALSE;
}
