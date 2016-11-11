#include <stdio.h>
#include <ncurses.h>
#include <panel.h>

#define LINES_GAME  30
#define COLS_GAME   100
#define LINES_LOGS LINES_GAME
#define COLS_LOGS   50
#define LINES_STATS  5
#define COLS_STATS COLS_GAME + COLS_LOGS
// Complier avec -lpanel -lncurses

void writeLog(char * message, int * line){

  char clearMsg[COLS_LOGS];

  int i;
  for(i = 0 ; i < LINES_LOGS ; i++) clearMsg[i] = ' ';

  mvprintw(*line, COLS_GAME+1, clearMsg);
  mvprintw(*line, COLS_GAME+1, message);

  if(*line >= LINES_LOGS - 2){
    (*line) = 1;
  } else {
    (*line)++;
  }

  refresh();
}

int main(){

  int key;
  int lineLog = 1;

  WINDOW *windows[3];
	PANEL  *panels[3];

  initscr();
  keypad(stdscr, TRUE);

  windows[0] = newwin(LINES_GAME, COLS_GAME, 0, 0);
  windows[1] = newwin(LINES_STATS, COLS_STATS, LINES_GAME, 0);
  windows[2] = newwin(LINES_LOGS, COLS_LOGS, 0, COLS_GAME);


  box(windows[0], 0, 0);
  box(windows[1], 0, 0);
  box(windows[2], 0, 0);

  panels[0] = new_panel(windows[0]);
  panels[1] = new_panel(windows[1]);
  panels[2] = new_panel(windows[2]);

  update_panels();
  doupdate();



  while((key = getch()) != 'q'){
    if(key == KEY_LEFT){
      writeLog("> Déplacement à gauche", &lineLog);
    }
    else if(key == KEY_RIGHT){
      writeLog("> Déplacement à droite", &lineLog);
    }
  }

	endwin();

}
