#include <stdio.h>
#include <stdlib.h>

#define N 500   /* N times N chessboard */

int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
int costShortestPath[N][N];

int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N);
}

void initialize() {
  int x, y;
  for (x=0; r < N; x++) {
    for (y=0; c < N; y++) {
      costShortestPath[x][y] = 999999; /* represents infinity */
    }
  }
}

knightAStar(int x0, y0, xG, yG){
	

int main(int argc, char *argv[]) {
  int x0,y0, xG,yG;
  do {
    printf("Start location (x,y) = "); fflush(stdout);
    scanf("%d %d", &x0, &y0);
  } while (!isValidLocation(x0,y0));
  do {
    printf("Goal location (x,y)  = "); fflush(stdout);
    scanf("%d %d", &xG, &yG);
  } while (!isValidLocation(xG,yG));

  printf("Length shortest path: %d\n", knightAStar(x0,y0, xG,yG));
  return 0;
}
