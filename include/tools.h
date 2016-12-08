/**
  * \file
  * \brief boite à outils du projet
  * \author MOTTIER Emeric
  * \author PELLOIN Valentin
  * \author TEYSSIER Titouan
  * \date 28 novembre 2016
  * \version 1.0
  */

#ifndef RANDOM_H
#define RANDOM_H

#include <time.h>
#include <stdlib.h>
#include <ncurses.h> // pour TRUE et FALSE

FILE * error;

int fileExist (char * fileName);

void initRandom (void);
int randab (int a, int b);
int didItHappen (int chance);

int min(int a, int b);
int max(int a, int b);

int isBetween(int val, int a, int b);
int intervalOverlaping(int a, int b, int c, int d);

int car2val (char car);
char up2low (char car);
char low2up (char car);

void err(char msg[]);
void initErr();
void unInitErr();

int numberLinesFile(char * file);
int maxColsFile(char * file);

#endif /* end of include guard: RANDOM_H */
