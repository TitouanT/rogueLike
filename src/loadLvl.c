/**
 * \file
 * \brief Programme de changement, chargement et sauvegarde de niveaux
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 1.3
 * \date 30 novembre 2016
 */
 
#include <stdio.h>

#include "cell.h" // OK
#include "mapConst.h" // OK
#include "lvl.h" // OK
#include "moves.h" // OK
#include "tools.h" // OK
#include "monstre.h" // OK
#include "character.h" // OK


char DOSSIER_SAUVEGARDE[30];
char NOM_NIVEAU[NB_LVL][30];
char NOM_POSITION[40];
char NOM_LVLDATA[40];

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
		sprintf(NOM_NIVEAU[i], "%s%i.txt", DOSSIER_SAUVEGARDE, i);
		err(NOM_NIVEAU[i]);
	}

	/*give the name for the position's file*/
	sprintf(NOM_POSITION, "%sposition.txt", DOSSIER_SAUVEGARDE);
	err(NOM_POSITION);

	/*give the name for the lvl data*/
	sprintf(NOM_LVLDATA, "%slvlData.txt", DOSSIER_SAUVEGARDE);
	err(NOM_LVLDATA);
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
  * \brief écrit dans un fichier les données sur les étages
  * \fn void writeLvlData (t_lvl tabLvl[NB_LVL], char * fileName)
  * \param tabLvl tableau qui contient chaque niveau
  * \param fileName nom du fichier à écrire
  */
void writeLvlData (t_lvl tabLvl[NB_LVL]) {
	err ("*** debut write lvl data ***");
	char msg[100];
	FILE * file = fopen (NOM_LVLDATA, "w");
	int i, j, line, column, height, width, nbRoom;
	sprintf(msg, "ecriture des infos dans : %s", NOM_LVLDATA);
	err(msg);



	for (i = 0; i < NB_LVL; i++) {
		sprintf (msg, "\técriture du nombre de pieces : %d/%d", i, NB_LVL-1);
		err(msg);
		nbRoom = tabLvl[i].nbRoom;
		fprintf (file, "\n%d\n", nbRoom);
		for (j = 0; j < nbRoom; j++) {
			sprintf (msg, "\t\técriture de la piece %d/%d", j, tabLvl[i].nbRoom - 1);
			err(msg);
			line = tabLvl[i].rooms[j].line;
			column = tabLvl[i].rooms[j].column;
			height = tabLvl[i].rooms[j].height;
			width = tabLvl[i].rooms[j].width;
			fprintf (file, "%d %d %d %d\n", line, column, height, width);
			//fprintf (file, "%d %d %d %d\n", 0, 0, 0, 0);
		}
	}
	fclose(file);
	err ("*** fin write lvl data ***");
}

/**
  * \brief lit dans un fichier les données sur les étages
  * \fn int readLvlData (t_lvl tabLvl[NB_LVL])
  * \param tabLvl tableau qui contiendra chaque niveau
  * \param fileName nom du fichier à lire
  * \return TRUE si la lecture c'est bien passée.
  * \return FALSE sinon.
  */
int readLvlData (t_lvl tabLvl[NB_LVL]) {
	err ("*** debut read lvl data ***");
	FILE * file = fopen (NOM_LVLDATA, "r");
	if (file == NULL) {
		err ("*** fin read lvl data (failure) ***");
		return FALSE; // failure
	}
	int i, j, nbRoom, line, column, height, width;
	for (i = 0; i < NB_LVL; i++) {

		fscanf (file, "%d", &nbRoom);
		tabLvl[i].nbRoom = nbRoom;

		for (j = 0; j < nbRoom; j++) {
			fscanf (file, "%d%d%d%d", &line, &column, &height, &width);
			tabLvl[i].rooms[j].line = line;
			tabLvl[i].rooms[j].column = column;
			tabLvl[i].rooms[j].height = height;
			tabLvl[i].rooms[j].width = width;
		}

	}
	fclose(file);
	err ("*** fin read lvl data (success) ***");
	return TRUE; // success
}


/**
	* \brief enregistre les stats du joueur dans un fichier "position.txt"
	*	\fn void writePosition (t_character player)
	* \param player joueur
	*/

void writePosition ( t_character player) {
	/* enregistre les paramètres du joueur dans les dossiers de sauvegardes */
	err("*** Début write Position ***");
	char msg[100];
	sprintf(msg, "ecriture de la position dans : %s", NOM_POSITION);
	FILE * positionFile;
	positionFile = fopen (NOM_POSITION, "w");
	if (positionFile == NULL) err("WTF ??");
	err(msg);
	fprintf (positionFile, "%i %i %i %i %i %i %i %i ", (player).line, (player).column, (player).lvl, (player).hp, (player).pw, (player).xp, (player).nbMove, (player).food);
	fprintf(positionFile, "%i %i ", (player).isSick, (player).hasFoundObj);
	fprintf(positionFile, "\n");
	fclose(positionFile);
	err("*** Fin write Position ***");
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
	* \brief Initialise la partie du joueur
	*	\fn void initGameMap (t_cell map[][COLUMNS], int choix, int choixFichierSauvegarde, t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonsters)
	* \param map Carte où se trouve le joueur
	* \param choix Choix 0 ou 1 (0 nouvelle partie, 1 reprendre partie)
	* \param choixFichierSauvegarde Choix de la sauvegarde
	* \param player Joueur
	*/

void initGameMap(t_cell map[LINES][COLUMNS], int choix, int choixFichierSauvegarde, t_character *player, t_monster monsters[NB_MONSTER_MAX], int * nbMonsters){
	/* Initialise les niveaux soit une nouvelle partie soit une sauvegarde */
	err("*** Debut init Game Map ****");
	int i;
	t_lvl lvlData[NB_LVL];
	initNameOfFile (choixFichierSauvegarde);


	if (choix == NEW_GAME) {
		err("Debut traitement new_game");
		//remove(NOM_POSITION);
		initStatRoom ();
		for(i = 0; i < NB_LVL; i++) {
			randomFloor(map, i);
			writeLvl(map, i);
		}

		if (queryLvlData (lvlData)) writeLvlData (lvlData);
		readLvl(map, 0);
		move2spawn(map, player, STAIRS_DOWN);
		
		createMonster (monsters, nbMonsters);
		err("Fin traitement new_game");
	}
	else{
		err("Debut traitement CONTINUE_GAME");
		readPosition(player);
		err( "lecture de l'étage");
		readLvl(map, player->lvl);
		readLvlData (lvlData);
		setLvlData (lvlData);
		err("Fin traitement CONTINUE_GAME");
	}
	err("*** Fin init Game Map ****");
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
		else{
			if(dir<0){
				move2spawn (map, player, STAIRS_UP);
			}
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
	err("*** Début Save Game ***");
	t_lvl lvlData[NB_LVL];
	if (queryLvlData (lvlData)) writeLvlData (lvlData);

	writeLvl (map, player->lvl);
	writePosition (*player);
	err("*** Fin Save Game ***");
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

void deleteGame(int choixFichierSauvegarde){
	char fileName[50];
	initNameOfFile (choixFichierSauvegarde);
	remove(NOM_POSITION);
	for(int i=0; i<NB_LVL;i++){
		remove(NOM_NIVEAU[i]);
	}
	remove(NOM_LVLDATA);

}
