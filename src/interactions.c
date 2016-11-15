#include <ncurses.h>
#include "global.h"


void traiterPorte(t_cell map[LINES][COLUMNS], t_character player, int key, WINDOW * win);
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win);
int askConfirmationToQuit(WINDOW * win);


/* Fonction principale d'intéraction avec l'utilisateur */
int handleInteraction(int key, t_cell map[LINES][COLUMNS], t_character *player, WINDOW * win_logs){

  int lineLog;

  switch (key) {
    case KEY_UP:    move_perso(UP,    map, player);  break;
    case KEY_DOWN:  move_perso(DOWN,  map, player);  break;
    case KEY_LEFT:  move_perso(LEFT,  map, player);  break;
    case KEY_RIGHT: move_perso(RIGHT, map, player);  break;

    case '\n':      traiterEntree(map, player, win_logs);      break;
    case 'o' :      traiterPorte (map, *player, key, win_logs); break;
    case 'c' :      traiterPorte (map, *player, key, win_logs); break;

    case 'q' : return FALSE;
    case 'Q' : return !askConfirmationToQuit(win_logs);

    default: addLog("Commande inconnue.", &lineLog, win_logs);
  }

  markDiscover(map, *player);

  return TRUE;

}


/* Traite l'ouverture et la fermeture d'une porte */
void traiterPorte(t_cell map[LINES][COLUMNS], t_character player, int key, WINDOW * win){


}

/* Traite l'appui sur la touche entrée */
void traiterEntree(t_cell map[LINES][COLUMNS], t_character *player, WINDOW *win){

  int lineLog = 0;

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
          addLog("Vous êtes déjà au niveau le plus haut !", &lineLog, win);
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
          addLog("Vous êtes déjà en bas !", &lineLog, win);
        }
        break;

      default: break ;

    }
  }
  else {
    addLog("Commande invalide.", &lineLog, win);
  }

}

int askConfirmationToQuit(WINDOW * win) {

	int key;
  int lineLog = 0;

	addLog("Etes-vous sur de vouloir quitter ? (y/n)", &lineLog, win);

	key = getch();

	switch (key) {
		case 'y': return TRUE;
		case 'n': return FALSE;

		default:
			addLog("Never mind", &lineLog, win);
			return FALSE;
	}

}
