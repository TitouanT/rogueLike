#include "global.h"

int main () {
	srand(time(NULL));
	t_cell map[LINES][COLUMNS];
	randomFloor (map, FALSE);
	displayFloor (map);
	return 0;
}
