#define N 4
#include <stdio.h>
#include <string.h>
void addLog (char * message) {
	while (strlen(message)>N) {
		printf("%.*s\n", N, message);
		message+=N;
	}
	printf("%s\n", message);
}
int main () {
	addLog ("abcdefghijklmnopqrstuvwxyz");
	return 0;
}
