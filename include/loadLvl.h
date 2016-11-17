#ifndef LOADLVL_H
#define LOADLVL_H

void readLvl (t_cell map[][COLUMNS], int nbLvl, char *dossier);
void writeLvl (t_cell map[][COLUMNS], int nbLvl, char *dossier);
void initGameMap(t_cell map[LINES][COLUMNS], int choix, int nbFichierSauvegarde);
void changeLvl(t_cell map[LINES][COLUMNS],t_character *player, int dir);
void saveGame(t_cell map[LINES][COLUMNS], t_character *player, int nbFichierSauvegarde);
int bFileSaveEmpty(int nbFichierSauvegarde);

#endif 
