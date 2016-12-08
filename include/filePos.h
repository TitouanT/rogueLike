/**
  * \file
  * \brief Structure abstraite de file sur des positions
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef FILEPOS_H
#define FILEPOS_H

#include "cell.h" // OK

/** initialise la file */
void file_init (void);

/** test si la file est vide */
int file_est_vide (void);

/** retire un élément de la file */
void file_retirer (t_pos *n);

/** ajoute un élément dans la file */
void file_ajouter (t_pos n);

/** suprime la file */
void file_supprimer (void);

#endif /* end of include guard: FILEPOS_H */
