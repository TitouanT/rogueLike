#include <stdio.h>

typedef union {int a ; int b; int tab;} t_g;

int main(){


  t_g titouan;
  titouan.a = 10;
  titouan.b = 12;
  titouan.tab = 25;

  printf("\n%i\n%i\n%i\n", titouan.a, titouan.b, titouan.tab);

}
