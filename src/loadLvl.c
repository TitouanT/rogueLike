/**
 * \file
 * \brief Programme de changement, chargement et sauvegarde de niveaux
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.3
 * \date 30 novembre 2016
 */

#include "global.h"
char DOSSIER_SAUVEGARDE[30];
char NOM_NIVEAU[NB_LVL][30];
char NOM_POSITION[40];

/**
	* \brief initialise les chemins d'accès au dossier de sauvegarde
	*	\fn void initNameOfFile (int choixDeSauvegarde)
	* \param choixDeSauvegarde
	*/
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

/**
	* \brief lit le fichier du niveau mis en paramètre dans le dossier de sauvegarde
	*	\fn void readLvl (t_cell map[][COLUMNS], int nbLvl)
	* \param map Carte où se trouve le joueur
	* \param nbLvl niveau où le joueur se situe
	*/

void readLvl ( t_cell map[][COLUMNS], int nbLvl) {
/* Lit un fichier dans un dossier donné */

	int i, j, k, type, state, isDiscovered, nbObject, object, isDiscoveredObject;
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

/**
	* \brief enregistre les stats du joueur dans un fichier "position.txt"
	*	\fn void writePosition (t_character player)
	* \param player joueur
	*/

void writePosition ( t_character player) {
/* enregistre les paramètres du joueur dans les dossiers de sauvegardes */
	FILE * positionFile;
	positionFile = fopen (NOM_POSITION, "w");
	fprintf (positionFile, "%i %i %i %i %i %i %i %i ", (player).line, (player).column, (player).lvl, (player).hp, (player).pw, (player).xp, (player).nbMove, (player).food);
	fprintf(positionFile, "%i %i ", (player).isSick, (player).hasFoundObj);
	fprintf(positionFile, "\n");
	fclose(positionFile);
}

/**
	* \brief lit les stats du joueur dans un fichier "position.txt"
	*	\fn void readPosition (t_character player)
	* \param player joueur
	*/

void readPosition ( t_character *player){
	err ("debut lecture position");
	FILE * positionFile = NULL;
	err("juste avant d' ouvrir le fichier position");

	positionFile = fopen (NOM_POSITION, "r");

	if (positionFile == NULL) err( "erreur de lecture !");

	fscanf(positionFile, "%i %i %i %i %i %i %i %i ", &(*player).line, &(*player).column, &(*player).lvl, &(*player).hp, &(*player).pw, &(*player).xp, &(*player).nbMove, &(*player).food);
	fscanf(positionFile, "%i %i ", &(*player).isSick, &(*player).hasFoundObj);

	fclose(positionFile);
	err("fin lecture position\n");
}

/**
	* \brief enregistre le fichier du niveau mis en paramètre dans le dossier de sauvegarde
	*	\fn void writeLvl (t_cell map[][COLUMNS], int nbLvl)
	* \param map Carte où se trouve le joueur
	* \param nbLvl niveau où le joueur se situe
	*/

void writeLvl ( t_cell map[][COLUMNS], int nbLvl) {
/* enregistre la partie soit dans le dossier temporaire ou dans les dossiers de sauvegardes */

	int i, j, k;
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

/**
	* \brief Initialise la partie du joueur
	*	\fn void initGameMap (t_cell map[][COLUMNS], int choix, int choixFichierSauvegarde, t_character *player)
	* \param map Carte où se trouve le joueur
	* \param choix Choix 0 ou 1 (0 nouvelle partie, 1 reprendre partie)
	* \param choixFichierSauvegarde Choix de la sauvegarde
	* \param player Joueur
	*/

void initGameMap(t_cell map[LINES][COLUMNS], int choix, int choixFichierSauvegarde, t_character *player){
/* Initialise les niveaux soit une nouvelle partie soit une sauvegarde */
	int i;
	initNameOfFile (choixFichierSauvegarde);


	if (choix == NEW_GAME) {
		remove(NOM_POSITION);
		for(i = 0; i < NB_LVL; i++) {
			randomFloor(map, i);
			writeLvl(map, i);
		}
		readLvl(map, 0);
		move2spawn(map, player, STAIRS_DOWN);
	}
	else{
		readPosition(player);
		err( "lecture de l'étage");
		readLvl(map, player->lvl);
	}
}

/**
	* \brief Permet de changer de niveaux
	*	\fn void readLvl (t_cell map[][COLUMNS], t_character *player, int dir)
	* \param map Carte où se trouve le joueur
	* \param player Joueur
	* \param dir Nombre d'étage que l'on descencd ou que l'on monte
	*/

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

/**
	* \brief Permet de sauvegarder la partie
	*	\fn void saveGame (t_cell map[][COLUMNS], t_character *playerr)
	* \param map Carte où se trouve le joueur
	* \param player Joueur
	*/

void saveGame(t_cell map[LINES][COLUMNS], t_character *player){
/* Fonction permettant de sauvegarder la partie à l'instant t */
	writeLvl (map, player->lvl);
	writePosition (*player);
}

/**
	* \brief Permet de vérifier si une partie existe dans un dossier de sauvegarde
	*	\fn void bFileSaveEmpty (int choixFichierSauvegarde)
	* \param choixFichierSauvegarde Numéro du dossier de sauvegarde
	*/

int bFileSaveEmpty(int choixFichierSauvegarde){
/* Fonction vérifiant si un dossier est vide */
	err("\n***Debut is File Save Empty***");
	int i;
	char fileName[50];
	sprintf(fileName, "./partie/sauvegarde%i/position.txt", choixFichierSauvegarde);
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
