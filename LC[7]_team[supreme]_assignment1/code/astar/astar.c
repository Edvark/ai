#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "queuesPos.h"

#define N 500   /* N times N chessboard */

int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};

/* The array isVisited indicates the positions that already have been visited.
 * The array shortestPath contains the shortest path from the starting position to [x][y].
 * They are used in many of the functions of the program. 
 * They have been declared as global variables to avoid having to pass them on in the function parameters.
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

/* This function is copied from Nestor, and is used to calculated effective branching factor */
double effectiveBranchingFactor(unsigned long states, int d) {
  /* approximates such that N=\Sum+{i=1}^{d} b^i */
  double lwb = 1;
  double upb = pow(states, 1.0/d);
  while (upb - lwb > 0.000001) {
    double mid = (lwb + upb) / 2.0;
    /* the following test makes use of the geometric series */
    if (mid*(1-pow(mid, d))/(1-mid) <= states) {  
      lwb = mid;
    } else {
      upb = mid;
    }
  }
  return lwb;
}

/* The function min returns the smallest of two integers.*/
int min(int x, int y){
	return ((x) <= (y)) ? (x) : (y);
}

/*Returns 1 if the location (x,y) is on the board, and has not been visited before.*/
int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N && isVisited[x][y] == 0);
}

/* This heuristic calculates the euclidian distance from (x,y) to the goal (xG, yG),
 * and divides it by the length of a single step (sqrt(2^2 + 1^2) = sqrt(5).
 * Since a knight has to stand in the boxes on the chessboard, its path is always longer than
 * the most direct path ignoring the boxes.
 */
double heuristicEuclidianDistance(int x, int y, int xG, int yG) {
	return sqrt((pow(xG-x,2) + pow(yG-y,2))/5);
}

/* This function returns the Manhattan distance between two points and divides it by the Manhattan step length (3) */
int heuristicManhattanDistance(int x, int y, int xG, int yG) {
	return ((abs(xG-x) + abs(yG-y))/3);
}

/* This function is used in the function that inserts Positions into the priority queue.
 * It has a parameter option so the user can choose between different heuristics */
int priority(Position s, Position t, int xG, int yG, int option){
	switch(option){
		case 0: /* A* Euclidean */
			if (shortestPath[s.x][s.y] + heuristicEuclidianDistance(s.x, s.y, xG, yG) < shortestPath[t.x][t.y] + heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return 1; //s is closer to goal than t, so s gets priority
			}
			if (shortestPath[s.x][s.y] + heuristicEuclidianDistance(s.x, s.y, xG, yG) > shortestPath[t.x][t.y] + heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
		case 1: /* A* Manhattan */
			if (shortestPath[s.x][s.y] + heuristicManhattanDistance(s.x, s.y, xG, yG) < shortestPath[t.x][t.y] + heuristicManhattanDistance(t.x, t.y, xG, yG)){
				return 1; 
				//s is closer to goal than t, so s gets priority
			}
			if (shortestPath[s.x][s.y] + heuristicManhattanDistance(s.x, s.y, xG, yG) > shortestPath[t.x][t.y] + heuristicManhattanDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
		case 2: /* Greedy Euclidean */
			if (heuristicEuclidianDistance(s.x, s.y, xG, yG) < heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return 1; //s is closer to goal than t, so s gets priority
			}
			if (heuristicEuclidianDistance(s.x, s.y, xG, yG) > heuristicEuclidianDistance(t.x, t.y, xG, yG)){
				return -1; 
				//t is closer to goal than s, so t gets priority
			}
			
		case 3: /* Greedy Manhattan */
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
	/* t and s are equally far away so their path length will be identical */
}

/*Inserts a  Position into a list, ordered by increasing values of f(n)=d(n)+h(n) */
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
	/*Its been added, not possible to be here, but its here to stop the compiler from complaining */
	return li;
}

int knightAStar(int x0, int y0, int xG, int yG, int option, unsigned long *numberOfStates){
   	
	Position pos;
	Position pos0 = {	/* The starting position*/
		.x = x0,
		.y = y0,
		.steps = 0
	};
	
	Queue q = newEmptyQueue();
	q.list = insertUnique(q.list, pos0, xG, yG, option); /* Add starting position to priority queue */
	
	while (!isEmptyQueue(q)){ 
		pos = dequeue(&q); //pos with minimal value of d+h because priority queue
		if (pos.x == xG && pos.y == yG){ /* The end goal is reached */
			return pos.steps;
		}
		isVisited[pos.x][pos.y] = 1; /* Remove the position from all positions that can be enqueued */
		for (int act = 0; act < 8; act++) { /* Make 8 new positions from the dequeued position*/
			Position new = {
				.y = pos.y + actions[act][0],
				.x = pos.x + actions[act][1],
				.steps = pos.steps + 1
			};
			if (isValidLocation(new.x, new.y)){ /* Check if the new position is a viable location*/
				shortestPath[new.x][new.y] = min(new.steps, shortestPath[new.x][new.y]); /* Update the shortest path array */
				*numberOfStates = *numberOfStates + 1; /* Increment the number of states made, this is needed to calculate branching factor */
				q.list = insertUnique(q.list, new, xG, yG, option); /* Insert it in the priority queue */
				
			}
		}
	}
	freeQueue(q);

	return 999999; /* This value resembles infinite, if this is returned, no path got to the end. */
}

int main(int argc, char *argv[]) {
	printf("\e[1;1H\e[2J"); fflush(stdout); /* Some dodgy code to clear the screen (throws compiler warnings) */
	int x0,y0, xG,yG, option;
	while (1){
		initialize(); /* Every time we want to calculate something, we need to reset the arrays */
		unsigned long numberOfStates = 0;
		printf("Which of the following heuristics would you like to use?:\n\n  0  A* Euclidian Distance\n  1  A* Manhattan Distance\n  2  Greedy Euclidian Distance\n  3  Greedy Manhattan Distance\n  4  None, quit\n\n: "); fflush(stdout);
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
		
		clock_t t = clock(); /* The time.h library is used to calculate the CPU time necessary for the calculation */
		int path = knightAStar(x0,y0, xG,yG, option, &numberOfStates);
		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC; /* Time taken in seconds */
		
		if (option == 2 || option == 3){ /* The greedy functions return an approximation, not guaranteed to be shortest. I wouldn’t want to lie to you */
			printf("\nApproximated shortest path: %d\n", path); fflush(stdout);
		} else {
			printf("\nShortest path length: %d\n", path); fflush(stdout);
		}

		printf("Effective Branching factor: %f\n", effectiveBranchingFactor(numberOfStates, path)); fflush(stdout);
		printf("Approximate CPU time: %.5fs\n\n", (time_taken)); fflush(stdout);
		printf("  0  Again\n  1  Quit\n\n: "); fflush(stdout);

		scanf("%d",&option);
		if (option == 1) {
			break;
		}
		printf("\n"); fflush(stdout);
	}
	return 0;
}
