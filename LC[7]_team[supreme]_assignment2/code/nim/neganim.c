#include <stdio.h>
#include <stdlib.h>

#define MAX 0
#define MIN 1

#define INFINITY 9999999

typedef struct pair {
	int move;
	int value;
}pair;

int max(int x, int y){
	if (y > x){
		return y;
	}
	return x;
}

int negaValue(pair state, int color){
	if (state == 1) {
		return color;
	}
	int move, value = -INFINITY;
	for (move = 1; move <= 3; move++) {
		if (state - move > 0) {
				value = max(value, color*negaValue(state - move,- 1 * color));
		}
	}
	return move;
}

int negaDecision(int state, int color) {
  int move, v;
    v = -INFINITY;
      for (move = 1; move <= 3; move++) {
      if (state.value - move > 0) { /* legal move */
        v = max(v, -1*negaValue(state.value-move,-1 * color));
        state.move = move
      }
    }
    return v;
}

void playNim(int state) {
  int turn = MAX;
  while (state != 1) {
    int action = negaDecision(state, 1);
    printf("%d: %s takes %d\n", state, 
           (turn==MAX ? "Max" : "Min"), action);
    state = state - action;
    turn = 1 - turn;
  }
  printf("1: %s looses\n", (turn==MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
  if ((argc != 2) || (atoi(argv[1]) < 3)) {
    fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
    fprintf(stderr, "<number of sticks> must be at least 3!\n");
    return -1;
  }

  playNim(atoi(argv[1]));

  return 0;
}
