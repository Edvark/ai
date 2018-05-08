#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function that does depth first search for current depth
int DFS(int start, int goal, int limit, int* cost, int* length, int* visited)
{
	*visited += 1;
	//base case
	if(limit == 0)
	{
		return (start == goal);
	}
	int currentLength = *length;
	int currentCost = *cost;
	
	//check each of the actions
	//each time we reset the cost and length
	*cost = currentCost + 1;
	*length = currentLength + 1;
	if(DFS(start+1, goal, limit - 1, cost, length, visited))
	{
		return 1;
	}
	
	*cost = currentCost + 2;
	*length = currentLength + 1;
	if(DFS(start*2, goal, limit - 1, cost, length, visited))
	{
		return 1;
	}
	
	*cost = currentCost + 2;
	*length = currentLength + 1;
	if(DFS(start*3, goal, limit - 1, cost, length, visited))
	{
		return 1;
	}
	
	*cost = currentCost + 1;
	*length = currentLength + 1;
	if(DFS(start-1, goal, limit - 1, cost, length, visited))
	{
		return 1;
	}
	
	*cost = currentCost + 3;
	*length = currentLength + 1;
	if(DFS(start/2, goal, limit - 1, cost, length, visited))
	{
		return 1;
	}
	
	*cost = currentCost + 3;
	*length = currentLength + 1;
	if(DFS(start/3, goal, limit - 1, cost, length, visited))
	{
		return 1;
	}
	
	return(start == goal);
}

//function that loops through all the depths <= limit
int iterativeDeepening(int start, int goal, int limit, int* cost, int* length, int* visited)
{
	for (int i = 0; i <= limit; i++)
	{
		*cost = 0;
		*length = 0;
		if(DFS(start, goal, i, cost, length, visited))
		{
			return 1;
		}
	}
	
	return 0;
	
}

int main(int argc, char *argv[]) {
  int start, goal, limit, cost, length;
  int visited = 0;
  
  //make sure the input is valid
  if (argc != 4) {
    fprintf(stderr, "Usage: %s [start] [goal] [depth]\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  //get input
  start = atoi(argv[1]);
  goal = atoi(argv[2]);
  limit = atoi(argv[3]);

  printf("Problem: route from %d to %d\n", start, goal);
  
  //check if we found a solution for the max limit
  if(iterativeDeepening(start, goal, limit, &cost, &length, &visited))
  {
	  printf("Cost: %d\nLength: %d\nNodes visited: %d\n", cost, length, visited);
  }
  else
  {
	  printf("No solution found for maximum limit: %d\n", limit);
  }
  
  
  return EXIT_SUCCESS;
}

