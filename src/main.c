#include "global.h"

int main () {
	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	randomFloor (map, TRUE);
	displayFloor (map);
	return 0;
}
