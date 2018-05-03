#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "queuesPos.h"

#define N 500   /* N times N chessboard */


int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};

/*The array isVisited indicates the positions that already have been visited.
 * The array shortestPath contains the shortest path from the starting position to [x][y].
 */
int isVisited[N][N];
int shortestPath[N][N];

void initialize(){
	/* 0 in isVisited indicates that you haven't been there, 1 indicates that you have been there.
	 * the shortestPath array is initialized with a large number resembling "infinite".
	 */
	for (int x = 0 ; x<N ; x++){
		for (int y = 0 ; y<N ; y++){
			isVisited[x][y] = 0;
			shortestPath[x][y] = 999999;
		}
	}
}

/* The function min returns the smallest of two integers.*/
int min(int x, int y){
	return ((x) <= (y)) ? (x) : (y);
}

/*Returns 1 if the location (x,y) is on the board, and has not been visited before.*/
int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N && isVisited[x][y] == 0);
}
/*This heuristic calculates the euclidian distance from (x,y) to the goal (xG, yG),
 * and divides it by the length of a single step (sqrt(2^2 + 1^2) = sqrt(5).
 * Since a knight has to stand in the boxes on the chessboard, its path is always longer than
 * the most direct path ignoring the boxes.
 */
double heuristicEuclidianDistance(int x, int y, int xG, int yG) {
	return sqrt((pow(xG-x,2) + pow(yG-y,2))/5);
}

int heuristicManhattanDistance(int x, int y, int xG, int yG) {
	return ((abs(xG-x) + abs(yG-y))/3);
}
int priority(Position s, Position t, int xG, int yG, int option){
	switch(option){
		case 0:
			if (shortestPath[s.x][s.y] + heuristicEuclidianDistance(s.x, s.y, xG, yG) < shortestPath[t.x][t.y] + heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return 1; //s is closer to goal than t, so s gets priority
			}
			if (shortestPath[s.x][s.y] + heuristicEuclidianDistance(s.x, s.y, xG, yG) > shortestPath[t.x][t.y] + heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
		case 1:
			if (shortestPath[s.x][s.y] + heuristicManhattanDistance(s.x, s.y, xG, yG) < shortestPath[t.x][t.y] + heuristicManhattanDistance(t.x, t.y, xG, yG)){
				return 1; 
				//s is closer to goal than t, so s gets priority
			}
			if (shortestPath[s.x][s.y] + heuristicManhattanDistance(s.x, s.y, xG, yG) > shortestPath[t.x][t.y] + heuristicManhattanDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
		case 2:
			if (heuristicEuclidianDistance(s.x, s.y, xG, yG) < heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return 1; //s is closer to goal than t, so s gets priority
			}
			if (heuristicEuclidianDistance(s.x, s.y, xG, yG) > heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
		case 3:
			if (heuristicManhattanDistance(s.x, s.y, xG, yG) < heuristicManhattanDistance(t.x, t.y, xG, yG)){
				return 1; 
				//s is closer to goal than t, so s gets priority
			}
			if (heuristicManhattanDistance(s.x, s.y, xG, yG) > heuristicManhattanDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
	}
	return 0;
	//t and s are equally far away, and t was added before s, so t has less or equal steps
}

/*Inserts a  Position into a list, ordered by increasing (euclidian distance)/(step size) to endgoal*/
List insertUnique(List li, Position s, int xG, int yG, int option){
	/*Empty list, so add item*/
	if(li == NULL){
		//printf("added %d at end of list\n", s.steps);
		return addItem(s,li);
	}
	/*Compares the current Position in the list, and the Position to be added*/
	int prio = priority(s, li->item, xG, yG, option);
	switch(prio){

		/* Add the new Position as its distance to endgoal is smaller*/
		case 1:
			//printf("added steps %d to queue\n",s.steps);
			return addItem(s,li);

		/*They are duplicates, and t was added before s, so t has less steps, don't add it*/
		case 0:
			//printf("duplicate\n");
			return li;

		/* New Position is further away from endgoal, so repeat function at next place in list*/
		case -1:
			li->next = insertUnique(li->next, s, xG, yG, option);
	}
	/*Its been added, not possible to be here, but its need just in case*/
	return li;
}

int knightAStar(int x0, int y0, int xG, int yG, int option){
   	
	Position pos;
	Position pos0 = {
		.x = x0,
		.y = y0,
		.steps = 0
	};
	
	Queue q = newEmptyQueue();
	q.list = insertUnique(q.list, pos0, xG, yG, option);
	
	while (!isEmptyQueue(q)){
		pos = dequeue(&q); //pos with minimal value of d+h because prior queue
		if (pos.x == xG && pos.y == yG){
			return pos.steps;
		}
		isVisited[pos.x][pos.y] = 1;
		for (int act = 0; act < 8; act++) {
			Position new = {
				.y = pos.y + actions[act][0],
				.x = pos.x + actions[act][1],
				.steps = pos.steps + 1
			};
			if (isValidLocation(new.x, new.y)){
				shortestPath[new.x][new.y] = min(new.steps, shortestPath[new.x][new.y]);
				q.list = insertUnique(q.list, new, xG, yG, option);
				
			}
		}
	}
	freeQueue(q);

	return 999999; //goal not reachable (not possible)
}

int main(int argc, char *argv[]) {
	int x0,y0, xG,yG, option;
	int busy = 1, random;
	while (busy == 1){
	initialize();
	printf("Which of the following heuristics would you like to use?:\n\n(0) A* Euclidian Distance\n(1) A* Manhattan Distance\n(2) Greedy Euclidian Distance\n(3) Greedy Manhattan Distance\n(4) None, quit\n"); fflush(stdout);
	scanf("%d", &option);
	if (option == 4){
		break;
	}
	do {
		printf("\nStart location (x,y) = "); fflush(stdout);
		scanf("%d %d", &x0, &y0);
	} while (!isValidLocation(x0,y0));
	do {
		printf("Goal location (x,y)  = "); fflush(stdout);
		scanf("%d %d", &xG, &yG);
	} while (!isValidLocation(xG,yG));
	
	if (option == 2 | option == 3){
		printf("Approximated shortest path: %d\n\n", knightAStar(x0,y0, xG,yG, option));
	} else {
		printf("Length shortest path: %d\n\n", knightAStar(x0,y0, xG,yG, option));
	}
	
	printf("(0) Again\n(1) Quit\n");
	scanf("%d",&random);
	if (random == 1) {break;}
	printf("\e[1;1H\e[2J");
	}
	printf("\e[1;1H\e[2J");
	return 0;
}
