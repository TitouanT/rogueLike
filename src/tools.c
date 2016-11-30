/**
  * \file
  * \brief boite à outils du projet
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#include "tools.h"
#include "mapConst.h"

/**
  * \brief initialise le tirage de nombres aléatoires
  * \fn void initRandom ()
  */
void initRandom () {
	srand(time(NULL));
}

/**
  * \brief choisis aléatoirement un nombre dans un intervalle
  * \fn int randab (int a, int b)
  * \param a borne min
  * \param b borne max
  * \return renvoi un nombre dans entre a et b exclus
  */
int randab (int a, int b) {
	int tmp;

	if (b < a) {
		tmp =   a;
		a   =   b;
		b   = tmp;
	}
	return (rand()%(b-a)) + a;
}

/**
  * \brief permet de savoir si un évènement c'est produit sachant sa probabilité
  * \fn int didItHappen (int chance)
  * \param chance probabilité (%) de réussite
  * \return TRUE si l'évènement se produit
  * \return FALSE sinon.
  */
int didItHappen (int chance) {
	return rand()%100 < chance;
}

/**
  * \brief transpose une valeur d'un intervalle donné
  *\ vers un autre intervalle de façons proportionnelle
  * \fn int mapping (int value, int min, int max, int destMin, int destMax)
  * \param value probabilité (%) de réussite
  * \param min La borne inférieur de l'intervalle de référence
  * \param max La borne supérieur de l'intervalle de référence
  * \param destMin La borne inférieur de l'intervalle cible
  * \param destMax La borne suérieur de l'intervalle cible
  * \return La valeur transposée
  */
int mapping (int value, int min, int max, int destMin, int destMax) {
	if (destMin > destMax) return destMax + destMin - mapping (value, min, max, destMax, destMin);
	return ((destMax - destMin) * (value - min) / (max - min)) + destMin;
				/*	max - min -> value - min
					destMax - destMin -> ? - destMin
					=> ? - destMin = (destMax - destMin) * (value - min) / (max - min)
					=> ? = ((destMax - destMin) * (value - min) / (max - min)) + destMin
				*/
}

/**
  * \brief calcul le minimum de deux entier
  * \fn int min (int a, int b)
  * \param a premier entier
  * \param b second entier
  * \return Le minimum de a et b
  */
int min (int a, int b) {
	if (a > b) return b;
	return a;
}

/**
  * \brief calcul le maximum de deux entier
  * \fn int max (int a, int b)
  * \param a premier entier
  * \param b second entier
  * \return Le maximum de a et b
  */
int max (int a, int b) {
	if (a > b) return a;
	return b;
}

/**
  * \brief test la présence d'un entier dans un intervalle
  * \fn int isBetween (int val, int a, int b)
  * \param val
  * \param a borne inférieur
  * \param b borne supérieur
  * \return TRUE si a appartient val [a, b]
  * \return FALSE sinon
  */
int isBetween (int val, int a, int b) { // return TRUE if val is in [a,b]
	if (a <= val && b >= val) return TRUE;
	else return FALSE;
}

/**
  * \brief test si l'intersection de deux intervalles est non nulle
  * \fn int intervalOverlaping (int a, int b, int c, int d)
  * \param a borne inférieur du premier intervalle
  * \param b borne supérieur du premier intervalle
  * \param c borne inférieur du second intervalle
  * \param d borne supérieur du second intervalle
  * \return TRUE si les intervalles se chevauchent
  * \return FALSE sinon
  */
int intervalOverlaping (int a, int b, int c, int d) { // return TRUE if the intersection of [a,b] and [c,d] is not empty
	if (isBetween (a, c, d) || isBetween (b, c, d) || isBetween (c, a, b) || isBetween (d, a, b)) return TRUE;
	else return FALSE;
}

/**
  * \brief convertit une lettre en position dans l'alphabet
  * \fn int car2val (char car)
  * \param car la lettre dont on veut la position dans l'alphabet
  * \return La position du caractere si c'est une lettre
  * \return -1 sinon
  */
int car2val (char car) {
	if (car >= 'a' && car <= 'z') return car - 'a';
	else if (car >= 'A' && car <= 'Z') return car - 'A';
	return -1;
}

/**
  * \brief convertit une lettre en minuscule
  * \fn char up2low (char car)
  * \param car la lettre que l'on veut en minuscule
  * \return La lettre correspondante en minuscule si c'est une lettre
  * \return -1 sinon
  */
char up2low (char car) {
	if (car >= 'A' && car <= 'Z') return car - 'A' + 'a';
	else if (car >= 'a' && car <= 'z') return car;
	else return -1;
}

/**
  * \brief convertit une lettre en majuscule
  * \fn char low2up (char car)
  * \param car la lettre que l'on veut en majuscule
  * \return La lettre correspondante en majuscule si c'est une lettre
  * \return -1 sinon
  */
char low2up (char car) {
	if (car >= 'A' && car <= 'Z') return car;
	else if (car >= 'a' && car <= 'z') return car - 'a' + 'A';
	else return -1;
}

void err (char msg[]) {
	if(PRINT_ERROR){
		FILE * err = fopen("err", "a");
		fprintf(err, "%s\n", msg);
		fclose(err);
	}
}
