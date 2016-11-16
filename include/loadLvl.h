#ifndef LOADLVL_H
#define LOADLVL_H

void readLvl (t_cell map[][COLUMNS], int nbLvl);
void writeLvl (t_cell map[][COLUMNS], int nbLvl, char *dossier);
void initGameMap(t_cell map[LINES][COLUMNS], int bsauvegarde);
void changeLvl(t_cell map[LINES][COLUMNS],t_character *player, int dir);


#endif 
