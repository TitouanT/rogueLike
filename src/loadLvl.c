#include "global.h"

void readLvl ( t_cell map[][COLUMNS], int nbLvl) { /// a mettre a jour
	int i, j, k, type, state, isDiscovered, nbObject, object;
	char fileName[40]="./partie/";
	char texte[20];
	sprintf(texte, "%i", nbLvl);
	strcat(fileName,texte);
	char texte2[20]=".txt";
	strcat(fileName,texte2);
	FILE * lvlFile;
	lvlFile = fopen (fileName, "r");
	//if (lvlFile == NULL) return FALSE;

	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLUMNS; j++) {
			fscanf (lvlFile, "%d%d%d%d", &type, &state, &isDiscovered, &nbObject);
			map[i][j].type = type;
			map[i][j].state = state;
			map[i][j].isDiscovered = isDiscovered;
			map[i][j].nbObject = nbObject;
			for (k = 0; k < nbObject; k++) {
				fscanf (lvlFile, "%d", &object);
				map[i][j].obj[k] = object;
			}
		}
	}
	fclose(lvlFile);
	//return TRUE;
}

void writeLvl ( t_cell map[][COLUMNS], int nbLvl) { //// a mettre a jours
	int i, j, k;
	char fileName[40]="./partie/";
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
				fprintf(lvlFile, "%d ", map[i][j].obj[k]);
			}

			fprintf(lvlFile, "\n");
		}
		fprintf(lvlFile, "\n");
	}
	fprintf(lvlFile, "\n");
	fclose(lvlFile);

}


void InitGameMap(t_cell map[LINES][COLUMNS]){
  for(int i=0;i<NB_LVL;i++){
    randomFloor(map);
    writeLvl(map,i);
  }
  readLvl(map,0);
}

void DOWN_LVL(t_cell map[LINES][COLUMNS],t_character *player){
	writeLvl(map,(player->lvl));
        (player->lvl)-- ;
        readLvl(map,(player->lvl));
        move2spawn(map, player, STAIRS_UP);
}

void UP_LVL(t_cell map[LINES][COLUMNS],t_character *player){
 	writeLvl(map,(player->lvl));
        (player->lvl)++;
        readLvl(map,(player->lvl));
        move2spawn(map, player, STAIRS_DOWN);
}

