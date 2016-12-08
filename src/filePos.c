/**
  * \file
  * \brief Structure abstraite de file sur des positions
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#include <stdlib.h> // pour free
#include "cell.h" // OK

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
	queue = tete = NULL;
}

/**
  * \brief test si la file est vide
  * \fn int file_est_vide ()
  * \return TRUE si la file est vide
  * \return FALSE sinon
  */
int file_est_vide () {
	return (tete == NULL);
}

/**
  * \brief retire un élément de la file
  * \fn void file_retirer (t_pos *n)
  * \param n adresse d'une position qui contiendra la position de l'élément retiré
  */
void file_retirer (t_pos *n) {
	t_element * tmp;
	if (tete != NULL) {
		tmp = tete;
		*n = tete->val;
		tete = tete->suiv;
		free(tmp);
	}
}

/**
  * \brief ajoute un élément dans la file
  * \fn void file_ajouter (t_pos n)
  * \param n position à ajouter dans la file
  */
void file_ajouter (t_pos n) {
	t_element * nouv = malloc (sizeof(t_element));
	nouv->suiv = NULL;
	nouv->val = n;
	if (tete == NULL) tete = nouv;
	else queue->suiv = nouv;
	queue = nouv;
}

/**
  * \brief supprime la file
  * procède à la libération de la mémoire alouée.
  * \fn void file_supprimer ()
  */
void file_supprimer () {
	t_element * tmp;
	while (tete != NULL) {
		tmp = tete;
		tete = tete->suiv;
		free(tmp);
	}
}
