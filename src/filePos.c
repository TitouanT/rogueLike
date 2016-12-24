/**
  * \file
  * \brief Structure abstraite de file sur des positions
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#include <stdlib.h> // OK

#include "cell.h"   // OK
#include "tools.h"  // OK

/**
  * \struct t_element
  * \brief décrit un élément de la file
  */
typedef struct element {
	t_pos val;              /*<! valeur stocké*/
	struct element * suiv;  /*<! pointeur sur l'élément suivant*/
} t_element;

/** pointeur sur la tete de la file */
t_element * tete;

/** pointeur sur la queue de la file */
t_element * queue;

/**
  * \brief initialise la file
  * \fn void file_init ()
  */
void file_init () {
	err ("<file_init/>", 0);
	queue = tete = NULL;
}

/**
  * \brief test si la file est vide
  * \fn int file_est_vide ()
  * \return TRUE si la file est vide
  * \return FALSE sinon
  */
int file_est_vide () {
	//err ("<file_est_vide>", +1);
	//err ("</file_est_vide>", -1);
	return (tete == NULL);
}

/**
  * \brief retire un élément de la file
  * \fn void file_retirer (t_pos *n)
  * \param n adresse d'une position qui contiendra la position de l'élément retiré
  */
void file_retirer (t_pos *n) {
	//err ("<file_retirer>", +1);
	t_element * tmp = NULL;
	if (tete != NULL) {
		tmp = tete;
		*n = tete->val;
		tete = tete->suiv;
		free(tmp);
		tmp = NULL;
	}
	//err ("</file_retirer>", -1);
}

/**
  * \brief ajoute un élément dans la file
  * \fn void file_ajouter (t_pos n)
  * \param n position à ajouter dans la file
  */
void file_ajouter (t_pos n) {
	//err ("<file_ajouter>", +1);
	t_element * nouv = malloc (sizeof(t_element));
	nouv->suiv = NULL;
	nouv->val = n;
	if (tete == NULL) tete = nouv;
	else queue->suiv = nouv;
	queue = nouv;
	//err ("</file_ajouter>", -1);
}

/**
  * \brief supprime la file
  * procède à la libération de la mémoire alouée.
  * \fn void file_supprimer ()
  */
void file_supprimer () {
	err ("<file_supprimer/>", 0);
	t_element * tmp = NULL;
	while (tete != NULL) {
		tmp = tete;
		tete = tete->suiv;
		free(tmp);
		tmp = NULL;
	}
}
