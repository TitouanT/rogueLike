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
char DOSSIER_SAUVEGARDE[30];
char NOM_NIVEAU[NB_LVL][30];
char NOM_POSITION[40];

void initNameOfFile (int choixDeSauvegarde) {
	err("\n***debut init name of file***");
	/*take the right place to save the game*/
	sprintf(DOSSIER_SAUVEGARDE, "./partie/sauvegarde%i/", choixDeSauvegarde);
	err(DOSSIER_SAUVEGARDE);
	/*give the name for level's file*/
	for (int i = 0; i < NB_LVL; i++) {
		sprintf(NOM_NIVEAU[i], "%s%i.txt",DOSSIER_SAUVEGARDE, i);
		err(NOM_NIVEAU[i]);
	}
	
	/*give the name for the position's file*/
	sprintf(NOM_POSITION, "%sposition.txt",DOSSIER_SAUVEGARDE);
	err(NOM_POSITION);
	err("***fin init name of file***\n");
}

void readLvl ( t_cell map[][COLUMNS], int nbLvl) {
/* Lit un fichier dans un dossier donné */

	int i, j, k, type, state, isDiscovered, nbObject, object, isDiscoveredObject;
	// char fileName[40];
	// strcpy(fileName,dossier);
	// char texte[20];
	// sprintf(texte, "%i", nbLvl);
	// strcat(fileName,texte);
	// char texte2[20]=".txt";
	// strcat(fileName,texte2);
	FILE * lvlFile;
	lvlFile = fopen (/*fileName*/ NOM_NIVEAU[nbLvl], "r");
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

void writePosition ( t_character player) {
/* enregistre les paramètres du joueur dans les dossiers de sauvegardes */
	// char fileName[50];
	// strcat(fileName,dossier);
	// char texte[40]="position.txt";
	// strcat(fileName,texte);
	FILE * positionFile;
	positionFile = fopen (NOM_POSITION, "w");
	fprintf (positionFile, "%i %i %i %i %i %i %i %i ", (player).line, (player).column, (player).lvl, (player).hp, (player).pw, (player).xp, (player).nbMove, (player).food);
	fprintf(positionFile, "%i ", (player).isSick);
	fprintf(positionFile, "\n");
	fclose(positionFile);
}

void readPosition ( t_character *player){
	err ("debut lecture position");
	// char fileName[50];
	// strcat(fileName,dossier);
	// char texte[40]="position.txt";
	// strcat(fileName,texte);
	FILE * positionFile = NULL;
	err("juste avant d' ouvrir le fichier position");
	
	positionFile = fopen (NOM_POSITION, "r");
	
	if (positionFile == NULL) err( "erreur de lecture !");
	
	fscanf(positionFile, "%i %i %i %i %i %i %i %i ", &(*player).line, &(*player).column, &(*player).lvl, &(*player).hp, &(*player).pw, &(*player).xp, &(*player).nbMove, &(*player).food);
	
	fclose(positionFile);
	err("fin lecture position\n");
}


void writeLvl ( t_cell map[][COLUMNS], int nbLvl) {
/* enregistre la partie soit dans le dossier temporaire ou dans les dossiers de sauvegardes */

	int i, j, k;
	// char fileName[40];
	// strcpy(fileName,dossier);
	// char texte[20];
	// sprintf(texte, "%i", nbLvl);
	// strcat(fileName,texte);
	// char texte2[20]=".txt";
	// strcat(fileName,texte2);
	FILE * lvlFile;
	lvlFile = fopen (NOM_NIVEAU[nbLvl], "w");
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
	int i;
	initNameOfFile (nbFichierSauvegarde);
	
	
	if (choix == NEW_GAME) {
		remove(NOM_POSITION);
		for(i = 0; i < NB_LVL; i++) {
			randomFloor(map, i);
			writeLvl(map, i);
		}
		readLvl(map, 0);
		move2spawn(map, player, STAIRS_DOWN);
		writePosition(*player);
	}
	else  {
		readPosition(player);
		err( "lecture de l'étage");
		readLvl(map, player->lvl);
	}
}

void changeLvl(t_cell map[LINES][COLUMNS], t_character *player, int dir){
/* Fonction permettant de changer de niveaux */

	
	if (isBetween (player->lvl + dir, 0, NB_LVL-1) ) {
		writeLvl (map, player->lvl);
		(player->lvl)+= dir;
		readLvl (map, player->lvl);
		if (dir > 0) {
			move2spawn (map, player, STAIRS_DOWN);
		}
		else {
			 move2spawn (map, player, STAIRS_UP);
		}
	}
}

void saveGame(t_cell map[LINES][COLUMNS], t_character *player){
/* Fonction permettant de sauvegarder la partie à l'instant t */
	writeLvl (map, player->lvl);
	writePosition (*player);
}

int bFileSaveEmpty(int nbFichierSauvegarde){ 
/* Fonction vérifiant si un dossier est vide */
	err("\n***Debut is File Save Empty***");
	int i;
	char fileName[50];
	sprintf(fileName, "./partie/sauvegarde%i/0.txt", nbFichierSauvegarde);
	// char texte[10];
	// sprintf(texte, "%i", nbFichierSauvegarde);
	// strcat(fileName,texte);
	// char texte2[10]="0.txt";
	// strcat(fileName,texte2);
	err(fileName);

	if(fopen(fileName, "r") == NULL){
		i=TRUE;
		remove(fileName);
	}else{
		i=FALSE;
	}
	err("***Fin is File Save Empty***\n");
	return i;
}
