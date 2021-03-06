#include <ncurses.h>        // OK
#include <stdlib.h>         // OK

#include "snake_list_ptr.h" // OK
#include "tools.h"          // OK


/**************************/
/* Parameters of the Game */
/**************************/

#define WAIT_TIME 50       // the time to wait for a user input
#define GROWTH 5           // length's gain when one food is eat
#define MAX_FOOD 10        // Maximum food quantity at the same time

// they both take a value in initGame()
int CAN_CROWL_ON_HIM;      // obvious
int CAN_GO_THROUGH_BORDER; // "

WINDOW *gWGame, *gWStats;
typedef enum {UP, DOWN, LEFT, RIGHT} t_dir;
int justForKonami = -1;
int lines, cols;

// initialize the ncurses library, the random, and print the screen
void initGame () {
	err("<initGame>", +1);
	timeout(WAIT_TIME);
	getmaxyx(stdscr, lines, cols);
	gWGame= subwin(stdscr, lines - 3, cols, 0, 0);
	gWStats= subwin(stdscr, 3, cols, lines - 3, 0);

	box(gWGame, ACS_VLINE, ACS_HLINE);
	box(gWStats, ACS_VLINE, ACS_HLINE);

	if (fileExist("./accentNeeded"))
		mvwprintw(gWGame, 0, 1, " Snake IT! --- <> with ♥ by T2 --- ");
	else
		mvwprintw(gWGame, 0, 1, " Snake IT! --- <> with love by T2 --- ");

	mvwprintw(gWStats, 0, 1, " Stats ");

	wrefresh(gWGame);
	wrefresh(gWStats);

	CAN_CROWL_ON_HIM      = (rand()%2 == 0) ? TRUE : FALSE;
	CAN_GO_THROUGH_BORDER = (rand()%2 == 0) ? TRUE : FALSE;


	err("</initGame>", -1);

}

// display the snake
void displaySnake (int isAlive, int currDir) {
	err("<displaySnake>", +1);
	t_pos pos;
	listPtr_move2head();
	listPtr_next ();
	while (!listPtr_isOut ()) {
		listPtr_readData (&pos);
		if (fileExist("./accentNeeded")) {
			switch (pos.body) {
				case HORIZONTAL:   mvwprintw(gWGame, pos.line, pos.col, "═"); break;
				case VERTICAL:     mvwprintw(gWGame, pos.line, pos.col, "║"); break;
				case TOP_LEFT:     mvwprintw(gWGame, pos.line, pos.col, "╔"); break;
				case TOP_RIGHT:    mvwprintw(gWGame, pos.line, pos.col, "╗"); break;
				case BOTTOM_LEFT:  mvwprintw(gWGame, pos.line, pos.col, "╚"); break;
				case BOTTOM_RIGHT: mvwprintw(gWGame, pos.line, pos.col, "╝"); break;
			}
		}
		else {
			switch (pos.body) {
				case HORIZONTAL:   mvwprintw(gWGame, pos.line, pos.col, "-"); break;
				case VERTICAL:     mvwprintw(gWGame, pos.line, pos.col, "|"); break;
				case TOP_LEFT:     mvwprintw(gWGame, pos.line, pos.col, "+"); break;
				case TOP_RIGHT:    mvwprintw(gWGame, pos.line, pos.col, "+"); break;
				case BOTTOM_LEFT:  mvwprintw(gWGame, pos.line, pos.col, "+"); break;
				case BOTTOM_RIGHT: mvwprintw(gWGame, pos.line, pos.col, "+"); break;
			}
		}
		listPtr_next();
	}

	listPtr_move2end();
	listPtr_readData(&pos);

	mvwprintw(gWGame, pos.line, pos.col, "*");

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
	err("</displaySnake>", -1);
}

// display some information about the game
void displayStatsSnake(int foodEat, int length, int foodQtt) {
	err("<displaySnake>", +1);
	/*mvwprintw(gWStats, 1, 1, "                                                    ");
	wprintw(gWStats, "                                                             ");
	wprintw(gWStats, "                                                             ");
	wrefresh(gWStats);
	mvwprintw(gWStats, 1, 1, "nourriture mangée: %d, longueur: %d, nourriture présente: %d, ", foodEat, length, foodQtt);

	if (CAN_CROWL_ON_HIM) wprintw(gWStats, "le serpent peut se manger lui-même");
	else wprintw(gWStats, "le serpent ne peut pas se manger lui-même");

	wprintw(gWStats, " %s ", (CAN_CROWL_ON_HIM == CAN_GO_THROUGH_BORDER) ? "ET" : "MAIS");

	if (CAN_GO_THROUGH_BORDER) wprintw(gWStats, "il peut aller au travers des murs");
	else wprintw(gWStats, "il ne peut pas aller au travers des murs");

	wprintw(gWStats, " | 'q' pour quitter");*/

	mvwprintw(gWStats, 1, 1, "nourriture mangée: %d, longueur: %d, nourriture présente: %d, cannibale(c): %3s, fantome(f): %3s | 'q' pour quitter | 'r' pour rejouer", foodEat, length, foodQtt, CAN_CROWL_ON_HIM ? "OUI" : "NON", CAN_GO_THROUGH_BORDER ? "OUI" : "NON");

	wrefresh(gWStats);
	err("</displaySnake>", -1);
}

// display all the food
void displayFood(t_pos * foods, int foodQtt) {
	err("<displayFood>", +1);

	int i;
	for (i = 0; i < foodQtt; i++) {
		wattron(gWGame, COLOR_PAIR(1));
		if (fileExist("./accentNeeded")) mvwprintw(gWGame, foods[i].line, foods[i].col, "♥");
		else mvwprintw(gWGame, foods[i].line, foods[i].col, "#");
		wattroff(gWGame, COLOR_PAIR(1));
	}
	err("</displayFood>", -1);

}

// add a random food at the i position
void randomFood(t_pos * foods, int i) {
	err("<randomFood>", +1);
	foods[i].line = rand()%(lines - 3 - 2) + 1;
	foods[i].col  = rand()%(cols - 2) + 1;
	err("</randomFood>", -1);
}

// return TRUE if the snake eat and update some stuff if he did
int eat(t_pos head, t_pos * foods, int foodQtt, int * foodEat, int * growth) {
	err("<eat>", +1);
	int i, eatSometh = FALSE;
	for (i = 0; i < foodQtt; i++) {
		if (head.line == foods[i].line && head.col == foods[i].col) {
			randomFood(foods, i);
			eatSometh = TRUE;
			(*foodEat)++;
			(*growth)+=GROWTH;
		}
	}
	err("</eat>", -1);
	return eatSometh;
}

void randPosOnWall (t_pos * pos, t_dir * currDir) {
	err("<randPosOnWall>", +1);
	int wall = rand()%4;
	int bounce = rand()%2;
	if (bounce == 0) {
		switch (wall) {
			case 0:
				*currDir = UP;
				pos -> line = lines - 5;
				pos -> col = randab(1, cols - 2);
				break;

			case 1:
				*currDir = DOWN;
				pos -> line = 1;
				pos -> col = randab(1, cols - 2);
				break;

			case 2:
				*currDir = RIGHT;
				pos -> line = randab(1, lines - 5);
				pos -> col = 1;
				break;

			case 3:
				*currDir = LEFT;
				pos -> line = randab(1, lines - 5);
				pos -> col = cols - 2;
				break;
		}
	}
	else {
		// *currDir = (*currDir == UP) ? DOWN : (*currDir == DOWN) ? UP : (*currDir == RIGHT) ? LEFT : RIGHT; // not eazy to understand..
		switch (*currDir) {
			case UP:
				*currDir = DOWN;
				(pos -> line)++;
				break;

			case DOWN:
				*currDir = UP;
				(pos -> line)--;
				break;

			case RIGHT:
				*currDir = LEFT;
				(pos -> col)--;
				break;

			case LEFT:
				*currDir = RIGHT;
				(pos -> col)++;
				break;
		}
	}
	err("</randPosOnWall>", -1);
}

void endGame() {
	err("<endGame>", +1);
	int i, j;
	for (i = 0; i < lines; i++)
		for (j = 0; j < cols; j++)
			mvprintw (i, j, " ");

	delwin(gWGame);
	delwin(gWStats);
	err("</endGame>", -1);
}

int snake(void) {
	err("<snake>", +1);
	/***************/
	/* Declaration */
	/***************/
	int key,
		foodEat,
		length,
		growth,
		foodQtt,
		continueGame;

	t_dir currDir,
		  prevDir;

	t_pos head,
		  prev,
		  end,
		  foods[MAX_FOOD];


	/******************/
	/* Initialisation */
	/******************/
replay:
	initGame();
	key = -1,
	foodEat = 0,
	length = 1,
	growth = 0,
	foodQtt = 1,
	continueGame = TRUE;
	currDir = RIGHT;
	head = (t_pos) {1, 1, HORIZONTAL};

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
				continueGame = FALSE;
				break;

			case 'r': // replay
				continueGame = FALSE;
				break;

			case ' ':
				growth += 1;
				break;

			case 'f':
				CAN_GO_THROUGH_BORDER = !CAN_GO_THROUGH_BORDER;
				break;

			case 'c':
				CAN_CROWL_ON_HIM = !CAN_CROWL_ON_HIM;
				break;

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
		}
		else {
			if (head.line >= lines - 3 - 1 || head.line <= 0 || head.col >= cols - 1 || head.col <= 0) {

				if (CAN_GO_THROUGH_BORDER && CAN_CROWL_ON_HIM) randPosOnWall(&head, &currDir);
				else {
					if (head.line >= lines - 3 - 1) head.line = 1;
					else if (head.line <= 0) head.line = lines - 3 - 2;

					if (head.col >= cols - 1) head.col = 1;
					else if (head.col <= 0) head.col = cols - 2;
				}
			}



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
		do {
			if (key == 'r'/*replay*/) {
				endGame();
				goto replay;
			}
			key = getch();
		} while (key != 'q');
	}

	endGame();
	err("</snake>", -1);
	return 0;
}



int konami (int key) {
	err("<konami>", +1);
	int code[10] = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, 'b', 'a'};
	justForKonami++;
	if (key == code[justForKonami]) {
		err("konami ! goodkey", 0);
		if (key == 'a') {
			snake();
			refresh();
			err("</konami>", -1);
			return TRUE;
		}
	}
	else {
		if (key == KEY_UP)justForKonami = 0;
		else justForKonami = -1;
		err("konami ! wrongKey", 0);
	}
	err("</konami>", -1);
	return FALSE;
}
