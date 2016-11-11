#include <stdlib.h>
#include "filePos.h"

typedef struct element {
	t_pos val;
	struct element * suiv;
} t_element;

t_element * tete, * queue;

void file_init () {
	queue = tete = NULL;
}

int file_est_vide () {
	return (tete == NULL);
}

void file_retirer (t_pos *n) {
	t_element * tmp;
	if (tete != NULL) {
		tmp = tete;
		*n = tete->val;
		tete = tete->suiv;
		free(tmp);
	}
}

void file_ajouter (t_pos n) {
	t_element * nouv = malloc (sizeof(t_element));
	nouv->suiv = NULL;
	nouv->val = n;
	if (tete == NULL) tete = nouv;
	else queue->suiv = nouv;
	queue = nouv;
}

void file_supprimer () {
	t_element * tmp;
	while (tete != NULL) {
		tmp = tete;
		tete = tete->suiv;
		free(tmp);
	}
}
