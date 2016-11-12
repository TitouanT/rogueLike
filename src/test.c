/*
 * \file
 * \brief Programme principal du jeu
 * \author MOTTIER Emeric
 * \author PELLOIN Valentin
 * \author TEYSSIER Titouan
 * \version 0.1
 * \date 12 novembre 2016
 */
#include <ncurses.h>

//typedef union {int a ; int b; int tab;} t_g;

int main(){


  t_g titouan;
  titouan.a = 10;
  titouan.b = 12;
  titouan.tab = 25;

  printf("\n%i\n%i\n%i\n", titouan.a, titouan.b, titouan.tab);

}
