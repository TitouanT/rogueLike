#include <ncurses.h>
#include "global.h"
#include "filePos.h"
#include "random.h"


void traiterPorte(t_cell map[LINES][COLUMNS],  t_character *player, int key, WINDOW * win, int *lineLog);
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win, int *lineLog);
int askConfirmationToQuit(WINDOW * win, int *lineLog);

// int rand_a_b(int a, int b){
//   srand(time(NULL));
//   return rand() % (b - a + 1) + a;
// }

void wrongKey(WINDOW * win, int *lineLog) {
	addLog("Never mind... (Nirvana)", lineLog, win);
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

		default: wrongKey(win_logs, lineLog);
	}

  markDiscover(map, *player);

  return TRUE;

}

int bIsValidDoor(t_cell map[LINES][COLUMNS], t_pos position){
  return (position.line >= 0 && position.column >= 0 && position.line < LINES && position.column < COLUMNS && map[position.line][position.column].type == DOORWAY);
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
		default: wrongKey(win, lineLog);
  }

  if(key == 'o'){

    if(bIsValidDoor(map, doorPos) && map[doorPos.line][doorPos.column].state == dCLOSE){
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

/* Traite l'appui sur la touche entrée */
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win, int *lineLog){


  if(map[player->line][player->column].nbObject > 0){

    switch (map[player->line][player->column].obj[0]) {

      case STAIRS_UP:
        if(player->lvl < NB_LVL - 1){
          UP_LVL(map,player);
        }
        else {
          addLog("Vous êtes déjà au niveau le plus haut !", lineLog, win);
        }
      break;

      case STAIRS_DOWN:
        if(player->lvl > 0){
          DOWN_LVL(map,player);
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

		default: wrongKey(win, lineLog);
	}
	return FALSE;
}
