#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "snake_list_ptr.h"
#include "tools.h"


/**************************/
/* Parameters of the Game */
/**************************/

#define WAIT_TIME 50                // the time to wait for a user input
#define GROWTH 5                    // length's gain when one food is eat
#define MAX_FOOD 10                 // Maximum food quantity in the game at the same time
#define CAN_CROWL_ON_HIM TRUE       // obvious
#define CAN_GO_THROUGH_BORDER TRUE  // "

WINDOW *gWGame, *gWStats;
typedef enum {UP, DOWN, LEFT, RIGHT} t_dir;
int justForKonami = -1;
int lines, cols;

// initialize the ncurses library, the random, and print the screen
void initGame () {
	// srand(time(NULL));
	// setlocale(LC_ALL, "");
	// initscr();
	timeout(WAIT_TIME); // if no no input then dont block
	//cbreak();
	//start_color();
	// keypad(stdscr, TRUE); // use of the arrow keys
	// noecho(); // no echo (obvious :p)
	// curs_set(0); // no cursor
	getmaxyx(stdscr, lines, cols);
	gWGame= subwin(stdscr, lines - 3, cols, 0, 0);
	gWStats= subwin(stdscr, 3, cols, lines - 3, 0);

	box(gWGame, ACS_VLINE, ACS_HLINE);
	box(gWStats, ACS_VLINE, ACS_HLINE);

	mvwprintw(gWGame, 0, 1, " Snake IT! --- <> with ♥ by T2 --- ");
	mvwprintw(gWStats, 0, 1, " Stats ");

	wrefresh(gWGame);
	wrefresh(gWStats);


}

// display the snake
void displaySnake (int isAlive, int currDir) {
	t_pos pos;


	listPtr_move2head();
	listPtr_next ();
	while (!listPtr_isOut ()) {
		listPtr_readData (&pos);
		switch (pos.body) {
			case HORIZONTAL:   mvwprintw(gWGame, pos.line, pos.col, "═"); break;
			case VERTICAL:     mvwprintw(gWGame, pos.line, pos.col, "║"); break;
			case TOP_LEFT:     mvwprintw(gWGame, pos.line, pos.col, "╔"); break;
			case TOP_RIGHT:    mvwprintw(gWGame, pos.line, pos.col, "╗"); break;
			case BOTTOM_LEFT:  mvwprintw(gWGame, pos.line, pos.col, "╚"); break;
			case BOTTOM_RIGHT: mvwprintw(gWGame, pos.line, pos.col, "╝"); break;
		}
		listPtr_next();
	}

	listPtr_move2end();
	listPtr_readData(&pos);
	
	mvwprintw(gWGame, pos.line, pos.col, "0");//"☣");
	//if (isAlive) mvwprintw(gWGame, pos.line, pos.col, "☣");
	listPtr_move2head();
	listPtr_readData (&pos);
	if (isAlive) {
		switch (currDir) {
			case UP:    mvwprintw(gWGame, pos.line, pos.col, "^"); break;
			case DOWN:  mvwprintw(gWGame, pos.line, pos.col, "v"); break;
			case RIGHT: mvwprintw(gWGame, pos.line, pos.col, ">"); break;
			case LEFT:  mvwprintw(gWGame, pos.line, pos.col, "<"); break;
		}
	}
	else mvwprintw(gWGame, pos.line, pos.col, "!");
}

// display some information about the game
void displayStatsSnake(int foodEat, int length, int foodQtt) {
	mvwprintw(gWStats, 1, 1, "nourriture mangée: %d, longueur: %d, nourriture présente: %d, ", foodEat, length, foodQtt);
	
	if (CAN_CROWL_ON_HIM) wprintw(gWStats, "le serpent peu se manger lui-même");
	else wprintw(gWStats, "le serpent ne peu pas se manger lui-même");
	
	wprintw(gWStats, " %s ", (CAN_CROWL_ON_HIM == CAN_GO_THROUGH_BORDER) ? "ET" : "MAIS");
	
	if (CAN_GO_THROUGH_BORDER) wprintw(gWStats, "il peut aller au travers des murs");
	else wprintw(gWStats, "il ne peut pas aller au travers des murs");
	
	
	wprintw(gWStats, " | 'q' pour quitter");
	
	
	wrefresh(gWStats);
}

// display all the food
void displayFood(t_pos * foods, int foodQtt) {
	int i;
	for (i = 0; i < foodQtt; i++) {
		wattron(gWGame, COLOR_PAIR(1));
		mvwprintw(gWGame, foods[i].line, foods[i].col, "♥");//"◉");
		wattroff(gWGame, COLOR_PAIR(1));
	}

}

// add a random food at the i position
void randomFood(t_pos * foods, int i) {
	foods[i].line = rand()%(lines - 3 - 2) + 1;
	foods[i].col  = rand()%(cols - 2) + 1;
}

// return TRUE if the snake eat and update some stuff if he did
int eat(t_pos head, t_pos * foods, int foodQtt, int * foodEat, int * growth) {
	int i, eatSometh = FALSE;
	for (i = 0; i < foodQtt; i++) {
		if (head.line == foods[i].line && head.col == foods[i].col) {
			randomFood(foods, i);
			eatSometh = TRUE;
			(*foodEat)++;
			(*growth)+=GROWTH;
		}
	}
	return eatSometh;
}

int snake(void) {
	err("*** Debut du snake ***");
	/**********************************/
	/* Declaration and Initialisation */
	/**********************************/
	int i, j,
	    key = -1,
	    foodEat = 0,
	    length = 1,
	    growth = 0,
	    foodQtt = 1,
	    continueGame = TRUE;

	t_dir currDir = RIGHT,
	      prevDir;

	t_pos head = {1, 1, HORIZONTAL},
	      prev,
		  end,
	      foods[MAX_FOOD];

	initGame();
	randomFood(foods, 0);

	listPtr_init ();
	listPtr_appendHead (head);

	/********************/
	/* Loop of the Game */
	/********************/

	while (continueGame) {
		// listen for the next player's action
		key = getch();
		prevDir = currDir;
		switch (key) {
			case 'q': // quit
				continueGame = FALSE; break;

			// the player can't go to the opposite direction
			case KEY_UP: // go up
				if (currDir != DOWN) currDir = UP;
				break;

			case KEY_DOWN: // go down
				if (currDir != UP) currDir = DOWN;
				break;

			case KEY_LEFT: // go left
				if (currDir != RIGHT) currDir = LEFT;
				break;

			case KEY_RIGHT: // go right
				if (currDir != LEFT) currDir = RIGHT;
				break;

			default: // anything else ? just continue
				break;
		}

		listPtr_move2head();
		listPtr_readData (&head);
		prev = head;

		// updating the position of the head
		switch (currDir) {
			case UP: head.line --; break;
			case DOWN: head.line ++; break;
			case RIGHT: head.col ++; break;
			case LEFT: head.col --; break;
		}

		// find the symbol to represent the part nex to the head
		if (prevDir == currDir) {
			if (currDir == UP || currDir == DOWN) prev.body = VERTICAL;
			else prev.body = HORIZONTAL;
		}
		else {
			if ((prevDir == UP && currDir == RIGHT) || (prevDir == LEFT && currDir == DOWN))
				prev.body = TOP_LEFT;

			else if ((prevDir == UP && currDir == LEFT) || (prevDir == RIGHT && currDir == DOWN))
				prev.body = TOP_RIGHT;

			else if ((prevDir == DOWN && currDir == RIGHT) || (prevDir == LEFT && currDir == UP))
				prev.body = BOTTOM_LEFT;

			else
				prev.body = BOTTOM_RIGHT;
		}
		// update the part next to the head
		listPtr_move2head();
		listPtr_removeElt (); // an update element primitive should be nice
		listPtr_appendHead (prev);

		// is it a legal movment ?
		if ((head.line >= lines - 3 - 1 || head.line <= 0 || head.col >= cols - 1 || head.col <= 0) && CAN_GO_THROUGH_BORDER == FALSE) {
			continueGame = FALSE;
		} else {
			if (head.line >= lines - 3 - 1) head.line = 1;
			else if (head.line <= 0) head.line = lines - 3 - 2;
			
			if (head.col >= cols - 1) head.col = 1;
			else if (head.col <= 0) head.col = cols - 2;
			
			
			
			if (eat(head, foods, foodQtt, &foodEat, &growth) && foodQtt < MAX_FOOD) {
				randomFood(foods, foodQtt);
				foodQtt++;
			}

			if (growth == 0) {
				listPtr_move2end ();
				listPtr_readData(&end);
				mvwprintw(gWGame, end.line, end.col, " ");
				listPtr_removeElt ();
			} else {
				growth--;
				length++;
			}
			
			
			if (listPtr_isInList(head) && CAN_CROWL_ON_HIM == FALSE) {
				continueGame = FALSE;
			} else {
				listPtr_appendHead (head);
				
				displaySnake(continueGame, currDir);
				displayFood(foods, foodQtt);
				wrefresh(gWGame);
				
				displayStatsSnake(foodEat, length, foodQtt);
			}
		}
	}

	/*******************/
	/* End of the Game */
	/*******************/

	listPtr_move2end ();
	listPtr_readData(&end);
	mvwprintw(gWGame, end.line, end.col, " ");
	listPtr_removeElt ();

	listPtr_appendHead (head);
	
	displaySnake(continueGame, currDir);
	wrefresh(gWGame);

	listPtr_removeList ();

	displayStatsSnake(foodEat, length, foodQtt);

	timeout(-1);
	if (key != 'q') {
		do key = getch();
		while (key != 'q');
	}

	//endwin();
	for (i = 0; i < lines; i++)
		for (j = 0; j < cols; j++)
			mvprintw (i, j, " ");

	delwin(gWGame);
	delwin(gWStats);

	err("*** Fin du Snake ***");
	return 0;
}

int konami (int key) {
	int code[10] = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, 'b', 'a'};
	justForKonami++;
	if (key == code[justForKonami]) {
		err("konami ! goodkey");
		if (key == 'a') {
			snake();
			refresh();
			return TRUE;
		}
	}
	else {
		if (key == KEY_UP)justForKonami = 0;
		else justForKonami = -1;
		err("konami ! wrongKey");
	}
	return FALSE;
}
