#include "tools.h"

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

// return the linear translation of value from [min, max] to [destMin, destMax]
int mapping (int value, int min, int max, int destMin, int destMax) {
	if (destMin > destMax) return destMax + destMin - mapping (value, min, max, destMax, destMin);
	return ((destMax - destMin) * (value - min) / (max - min)) + destMin;
				/*	max - min -> value - min
					destMax - destMin -> ? - destMin
					=> ? - destMin = (destMax - destMin) * (value - min) / (max - min)
					=> ? = ((destMax - destMin) * (value - min) / (max - min)) + destMin
				*/
}

int car2val (char car) {
	if (car >= 'a' && car <= 'z') return car - 'a';
	else if (car >= 'A' && car <= 'Z') return car - 'A';
	return -1;
}

char up2low (char car) {
	if (car >= 'A' && car <= 'Z') return car - 'A' + 'a';
	else if (car >= 'a' && car <= 'z') return car;
	else return -1;
}

char low2up (char car) {
	if (car >= 'A' && car <= 'Z') return car;
	else if (car >= 'a' && car <= 'z') return car - 'a' + 'A';
	else return -1;
}
