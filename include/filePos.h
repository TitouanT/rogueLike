#ifndef FILEPOS_H
#define FILEPOS_H


typedef struct {int line, column;} t_pos;

void file_init (void);
int file_est_vide (void);
void file_retirer (t_pos *n);
void file_ajouter (t_pos n);
void file_supprimer (void);

#endif /* end of include guard: FILEPOS_H */
