#include "random.h"

void initRandom () {
	srand(time(NULL));
}

int randab (int a, int b) { // return a number in [a,b[
	int tmp;

	if (b <= a) {
		tmp =   a;
		a   =   b;
		b   = tmp;
	}
	return (rand()%(b-a)) + a;
}

int didItHappen (int chance) {
	return rand()%100 < chance;
}
