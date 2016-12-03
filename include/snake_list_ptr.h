/* Titouan Teyssier - 11/27/16 */

#ifndef LIST_PTR_H
#define LIST_PTR_H


typedef enum {HORIZONTAL, VERTICAL, TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT} t_body;
typedef struct {
	int line, col;
	t_body body;
} t_pos;


void listPtr_appendEnd (t_pos data);
void listPtr_appendHead (t_pos data);
void listPtr_appendLeft (t_pos data);
void listPtr_appendRight (t_pos data);
void listPtr_init (void);
int  listPtr_isEmpty (void);
int  listPtr_isInList (t_pos data); 
int  listPtr_isOut (void);
void listPtr_move2end (void);
void listPtr_move2head (void);
void listPtr_next (void);
void listPtr_prev (void);
void listPtr_readData (t_pos * data);
void listPtr_removeElt (void);
void listPtr_removeList (void);

#endif /* end of include guard: LIST_PTR_H */
