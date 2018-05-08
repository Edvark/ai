#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"

#define RANGE 1000000

Fringe insertValidSucc(Fringe fringe, int value, int cost, int action, short paths[RANGE][2]) 
{
  State s;
 
  //make sure the value is not out of bounds, and check if the thing we found is more efficient
  if ((value < 0) || (value > RANGE) || (paths[value][0] != 0 && cost >= paths[value][1] && fringe.mode == HEAP) || (paths[value][0] != 0 && fringe.mode != HEAP)) {
    return fringe;
  }
 
  //insert the actions and costs at the right position in the array
  paths[value][0] = action;
  paths[value][1] = cost;
  s.value = value;
  s.cost = cost;
  return insertFringe(fringe, s);
}

int search(int mode, int start, int goal, short paths[RANGE][2]) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value;
  int cost = 0;
  
  fringe = makeFringe(mode);
  state.value = start;
  state.cost = 0;
  paths[start][0] = 1;
  fringe = insertFringe(fringe, state);
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    
    fringe = removeFringe(fringe, &state);
    visited++;
    /* is state the goal? */
    value = state.value;
    if (value == goal) {
      goalReached = 1;
      cost = state.cost;
      break;
    }
    /* insert neighbouring states */
    //each time we also check for the goal
    fringe = insertValidSucc(fringe, value+1, state.cost+1, 1, paths); /* rule n->n + 1      */
    fringe = insertValidSucc(fringe, 2*value, state.cost+2, 2, paths); /* rule n->2*n        */
    fringe = insertValidSucc(fringe, 3*value, state.cost+2, 3, paths); /* rule n->3*n        */
    fringe = insertValidSucc(fringe, value-1, state.cost+1, 4, paths); /* rule n->n - 1      */
	fringe = insertValidSucc(fringe, value/2, state.cost+3, 5 + value%2, paths); /* rule n->floor(n/2) */
	fringe = insertValidSucc(fringe, value/3, state.cost+3, 7 + value%3, paths); /* rule n->floor(n/3) */
    
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);
  return cost;  
}

//function that prints the path to the goal
int printPath(short paths[RANGE][2], int start, int goal)
{
	//base case
	if(goal == start)
	{
		printf("%d ", goal);
		return 0;
	}
	
	int lenght = 0;
	
	//we go backwards through the matrix and print the path
	switch (paths[goal][0])
	{
		case 0:
			return -1;
		case 1:
			lenght = printPath(paths, start, goal - 1) + 1;
			printf("(+1)-> %d ", goal);
			return lenght;
		case 2:
			lenght = printPath(paths, start, goal / 2) + 1;
			printf("(*2)-> %d ", goal);
			return lenght;
		case 3:
			lenght = printPath(paths, start, goal / 3) + 1;
			printf("(*3)-> %d ", goal);
			return lenght;
		case 4:
			lenght = printPath(paths, start, goal + 1) + 1;
			printf("(-1)-> %d ", goal);
			return lenght;
		case 5:
		case 6:
			lenght = printPath(paths, start, goal * 2 + paths[goal][0] - 5) + 1;
			printf("(/2)-> %d ", goal);
			return lenght;
		case 7:
		case 8:
		case 9:
			lenght = printPath(paths, start, goal * 3 + paths[goal][0] - 7) + 1;
			printf("(/3)-> %d ", goal);
			return lenght;
	}
	return -1;
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  
  //make a matrix with two rows
  //in the first row, at each index we have the corresponding action we used to get to that index
  //in the second row, at each index we have the cost required to reach that index
  short paths[RANGE][2] = {0};
  
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }

  start = 0;
  goal = 42;
  if (argc == 3) {
    goal = atoi(argv[2]);
  } else if (argc == 4) {
    start = atoi(argv[2]);
    goal = atoi(argv[3]);
  }

  printf("Problem: route from %d to %d\n", start, goal);
  
  int cost = search(fringetype, start, goal, paths); 
  printf("\nlenght: %d, cost: %d \n", printPath(paths, start, goal), cost);
  return EXIT_SUCCESS;
}
