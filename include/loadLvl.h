/**
  * \file
  * \brief Fonctions de sauvegarde et chargement de niveaux
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 05 decembre 2016
  * \version 3.14159265359
  */

#ifndef LOADLVL_H
#define LOADLVL_H

#include "cell.h" // OK
#include "mapConst.h" // OK
#include "character.h" // OK
#include "monstre.h"

void initGameMap(t_cell map[LINES][COLUMNS], int choix, int nbFichierSauvegarde, t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonsters);
void changeLvl(t_cell map[LINES][COLUMNS],t_character *player, int dir);
void saveGame(t_cell map[LINES][COLUMNS], t_character *player);
int bFileSaveEmpty(int nbFichierSauvegarde);
void deleteGame(int choixFichierSauvegarde);

#endif
