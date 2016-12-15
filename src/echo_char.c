#include <ncurses.h>
#include <locale.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

int main(){
  setlocale(LC_CTYPE, "");
  //initscr();
  //raw();
  //noecho();
  //keypad(stdscr, TRUE);

  int ch;

  //ch = getch();
  //endwin();
  int ms = 5000;
int freq = 440;
int fd = 0;
ioctl(fd, KDMKTONE, (ms<<16 | 1193180/freq));
  //printf("Keycode: %i\n",ch);

  return 0;
}
