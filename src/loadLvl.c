/**
 * \file
 * \brief Programme de changement, chargement et sauvegarde de niveaux
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.1
 * \date 19 novembre 2016
 */

#include "global.h"
char DOSSIERPARTIE[40]="./partie/";
char DOSSIERSAUVEGARDE[40]="./partie/sauvegarde";

void readLvl ( t_cell map[][COLUMNS], int nbLvl, char dossier[]) {
/* Lit un fichier dans un dossier donné */

	int i, j, k, type, state, isDiscovered, nbObject, object, isDiscoveredObject;
	char fileName[40];
	strcpy(fileName,dossier);
	char texte[20];
	sprintf(texte, "%i", nbLvl);
	strcat(fileName,texte);
	char texte2[20]=".txt";
	strcat(fileName,texte2);
	FILE * lvlFile;
	lvlFile = fopen (fileName, "r");
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			fscanf (lvlFile, "%d%d%d%d", &type, &state, &isDiscovered, &nbObject);
			map[i][j].type = type;
			map[i][j].state = state;
			map[i][j].isDiscovered = isDiscovered;
			map[i][j].nbObject = nbObject;
			for (k = 0; k < nbObject; k++) {
				fscanf (lvlFile, "%d %d", &object, &isDiscoveredObject);
				map[i][j].obj[k].type = object;
				map[i][j].obj[k].isDiscovered = isDiscoveredObject;
			}
		}
	}
	fclose(lvlFile);
}

void writePosition ( t_character player, char dossier[]) {
/* enregistre les paramètres du joueur dans les dossiers de sauvegardes */
	char fileName[50];
	strcat(fileName,dossier);
	char texte[40]="position.txt";
	strcat(fileName,texte);
	FILE * positionFile;
	positionFile = fopen (fileName, "w");
	fprintf (positionFile, "%i %i %i %i %i %i %i %i ", (player).line, (player).column, (player).lvl, (player).hp, (player).pw, (player).xp, (player).nbMove, (player).food);
	fprintf(positionFile, "%i ", (player).isSick);
	fprintf(positionFile, "\n");
	fclose(positionFile);
}

void readPosition ( t_character *player, char dossier[]){
	char fileName[50];
	strcat(fileName,dossier);
	char texte[40]="position.txt";
	strcat(fileName,texte);
	FILE * positionFile;
	positionFile = fopen (fileName, "r");
	fscanf(positionFile, "%i %i %i %i %i %i %i %i ", &(*player).line, &(*player).column, &(*player).lvl, &(*player).hp, &(*player).pw, &(*player).xp, &(*player).nbMove, &(*player).food);
	fclose(positionFile);
}


void writeLvl ( t_cell map[][COLUMNS], int nbLvl, char dossier[]) {
/* enregistre la partie soit dans le dossier temporaire ou dans les dossiers de sauvegardes */

	int i, j, k;
	char fileName[40];
	strcpy(fileName,dossier);
	char texte[20];
	sprintf(texte, "%i", nbLvl);
	strcat(fileName,texte);
	char texte2[20]=".txt";
	strcat(fileName,texte2);
	FILE * lvlFile;
	lvlFile = fopen (fileName, "w");
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			fprintf (lvlFile, "%d %d %d %d ", map[i][j].type, map[i][j].state, map[i][j].isDiscovered, map[i][j].nbObject);

			for (k = 0; k < map[i][j].nbObject; k++) {
				fprintf(lvlFile, "%d %d ", map[i][j].obj[k].type, map[i][j].obj[k].isDiscovered);
			}

			fprintf(lvlFile, "\n");
		}
		fprintf(lvlFile, "\n");
	}
	fprintf(lvlFile, "\n");
	fclose(lvlFile);

}


void initGameMap(t_cell map[LINES][COLUMNS], int choix, int nbFichierSauvegarde, t_character *player){
/* Initialise les niveaux soit une nouvelle partie soit une sauvegarde */
		char texte[20];
		sprintf(texte, "%i", nbFichierSauvegarde);
		strcat(DOSSIERSAUVEGARDE,texte);
		char texte2[20]="/";
		strcat(DOSSIERSAUVEGARDE,texte2);
			if(choix==0){
				for(int i=0;i<NB_LVL;i++){
					randomFloor(map, i);
					writeLvl(map,i,DOSSIERPARTIE);
				}
				readLvl(map,(player->lvl),DOSSIERPARTIE);
				move2spawn(map, player, STAIRS_DOWN);
			}else{
				for(int i=0;i<NB_LVL;i++){       // on copie tout dans le dossier sauvegarde
						readLvl(map,i,DOSSIERSAUVEGARDE);
						writeLvl(map,i,DOSSIERPARTIE);
				}
				readPosition(*&player,DOSSIERSAUVEGARDE);
				readLvl(map,(player->lvl),DOSSIERPARTIE);
			}
}

void changeLvl(t_cell map[LINES][COLUMNS], t_character *player, int dir){
/* Fonction permettant de changer de niveaux */

	if (isBetween(player->lvl + dir, 0, NB_LVL-1) ) {
		writeLvl(map,(player->lvl),DOSSIERPARTIE);
		(player->lvl)+= dir;
		readLvl(map,(player->lvl),DOSSIERPARTIE);
		if (dir > 0){
			move2spawn(map, player, STAIRS_DOWN);
		}else{
			 move2spawn(map, player, STAIRS_UP);
		}
	}
}

void saveGame(t_cell map[LINES][COLUMNS], t_character *player){
/* Fonction permettant de sauvegarder la partie à l'instant t */
	writeLvl(map,(player->lvl),DOSSIERPARTIE);
	for(int i=0;i<NB_LVL;i++){       // on copie tout dans le dossier sauvegarde
		readLvl(map,i,DOSSIERPARTIE);
		writeLvl(map,i,DOSSIERSAUVEGARDE);
	}
	writePosition(*player,DOSSIERSAUVEGARDE);
	readLvl(map,(player->lvl),DOSSIERPARTIE);       // on revient où on en était
}

int bFileSaveEmpty(int nbFichierSauvegarde){
/* Fonction vérifiant si un dossier est vide */
	int i;
	char fileName[50];
	strcpy(fileName,DOSSIERSAUVEGARDE);
	char texte[10];
	sprintf(texte, "%i", nbFichierSauvegarde);
	strcat(fileName,texte);
	char texte2[10]="/0.txt";
	strcat(fileName,texte2);

	if(fopen(fileName, "r") == NULL){
		i=FALSE;
	}else{
		i=TRUE;
	}
	return i;
}
