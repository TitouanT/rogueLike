#ifndef GLOBAL_H
#define GLOBAL_H

#define TRUE 1
#define FALSE 0
#define LINES 20
#define COLUMNS 90

#include "cell.h"

typedef struct {int line, column, lvl} t_pos;
typedef enum {UP, DOWN, LEFT, RIGHT} t_dir;

#endif /* end of include guard: GLOBAL_H */
