#include <ncurses.h>
#include <locale.h> 

int main(){
  setlocale(LC_CTYPE, "");
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);

  int ch;

  ch = getch();
  endwin();

  printf("Keycode: %i\n",ch);

  return 0;
}
