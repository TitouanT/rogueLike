#ifndef RANDOM_H
#define RANDOM_H

#include <time.h>
#include <stdlib.h>

void initRandom (void);
int randab (int a, int b);
int didItHappen (int chance);
int car2val (char car);
char up2low (char car);
char low2up (char car);

#endif /* end of include guard: RANDOM_H */
