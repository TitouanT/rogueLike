#include <ncurses.h>
#include "global.h"
#include "filePos.h"


void traiterPorte(t_cell map[LINES][COLUMNS],  t_character *player, int key, WINDOW * win, int *lineLog);
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win, int *lineLog);
int askConfirmationToQuit(WINDOW * win, int *lineLog);

int rand_a_b(int a, int b){
  srand(time(NULL));
  return rand() % (b - a + 1) + a;
}

/* Fonction principale d'intéraction avec l'utilisateur */
int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs, int *lineLog){


  switch (key) {
    case KEY_UP:    move_perso(UP,    map, player);  break;
    case KEY_DOWN:  move_perso(DOWN,  map, player);  break;
    case KEY_LEFT:  move_perso(LEFT,  map, player);  break;
    case KEY_RIGHT: move_perso(RIGHT, map, player);  break;

    case '\n':      traiterEntree(map, player,  win_logs,     lineLog); break;
    case 'o' :      traiterPorte (map, player, key, win_logs, lineLog); break;
    case 'c' :      traiterPorte (map, player, key, win_logs, lineLog); break;

    case 'q' : return FALSE;
    case 'Q' : return !askConfirmationToQuit(win_logs, lineLog);

    default: addLog("Commande inconnue.", lineLog, win_logs);
  }

  markDiscover(map, *player);

  return TRUE;

}


int bIsDoorClosed(t_cell map[LINES][COLUMNS], t_pos position){

  if(position.line > 0 && map[position.line][position.column].type == DOORWAY){
    if(map[position.line][position.column].state == dCLOSE){
      return TRUE;
    }
  }

  return FALSE;

}

/* Traite l'ouverture et la fermeture d'une porte */
void traiterPorte(t_cell map[LINES][COLUMNS], t_character *player, int key, WINDOW * win, int *lineLog){

  int direction;
  t_pos doorPos = {player->line, player->column};

  addLog("Quelle porte voulez vous ouvrir ?", lineLog, win);
  addLog("  (flèche haute, basse, gauche, droite)", lineLog, win);

  direction = getch();

  switch (direction) {
    case KEY_UP    : (doorPos.line)--;   break;
    case KEY_DOWN  : (doorPos.line)++;   break;
    case KEY_LEFT  : (doorPos.column)--; break;
    case KEY_RIGHT : (doorPos.column)++; break;
  }

  if(bIsDoorClosed(map, doorPos)){
    if(rand_a_b(0,3) == 0) {
      addLog("Vous venez d'enfoncer cette porte.", lineLog, win);
      addLog("Recommencez pour l'ouvrir entièrement !", lineLog, win);
    }
    else map[doorPos.line][doorPos.column].state = dOPEN;

    (player->nbMove)++;
  }
  else{
    addLog("Porte inexistante !", lineLog, win);
  }


}

/* Traite l'appui sur la touche entrée */
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win, int *lineLog){


  if(map[player->line][player->column].nbObject > 0){

    switch (map[player->line][player->column].obj[0]) {

      case STAIRS_UP:
        if(player->lvl < NB_LVL){
          writeLvl(map,(player->lvl));
          (player->lvl)++;
          readLvl(map,(player->lvl));
          move2spawn(map, player);
        }
        else {
          addLog("Vous êtes déjà au niveau le plus haut !", lineLog, win);
        }
      break;

      case STAIRS_DOWN:
        if(player->lvl > 0){
          writeLvl(map,(player->lvl));
          (player->lvl)-- ;
          readLvl(map,(player->lvl));
          move2Stairs_UP(map, player);
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

int askConfirmationToQuit(WINDOW * win, int *lineLog) {

	int key;

	addLog("Etes-vous sur de vouloir quitter ? (y/n)", lineLog, win);

	key = getch();

	switch (key) {
		case 'y': return TRUE;
		case 'n': return FALSE;

		default:
			addLog("Never mind", lineLog, win);
			return FALSE;
	}

}
