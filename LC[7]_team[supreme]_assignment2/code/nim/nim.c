#include <stdio.h>
#include <stdlib.h>

#define MAX 0
#define MIN 1

#define INFINITY 9999999

int negaMax(int state, int color, int * bestMove, int transpositionTable[200])
{
	//is terminal
	if (state == 1) 
	{
		//psuedocode states return -color, but that doesn't work, 
		//if we return -1 it works exactly the same (up untill 20) as the algorithm in the given code
		return -1;
	}
	
	//return from the transpositionTable if it excist
	if(transpositionTable[100 + color*state] != 0)
	{
		int winOrLoose = - 1 * negaMax(state-transpositionTable[100 + color*state], -1 * color, bestMove, transpositionTable);
		*bestMove = transpositionTable[100 + color*state];
		return winOrLoose;
	}
	
	int v = -INFINITY;
	int currentBestMove = 1;
	for (int move = 1; move <= 3; move++) 
	{
		
		if (state - move > 0) 
		{
			int m = - 1 * negaMax(state-move, -1 * color, bestMove, transpositionTable);
			if(m > v)
			{
				currentBestMove = move;
				v = m;
			}
		}
	}
	transpositionTable[100 + color*state] = currentBestMove;
	*bestMove = currentBestMove;
	return v;
}

void playNim(int state) 
{
	int turn = 0;
	int transpositionTable[200] = { 0 };
	
	while (state != 1) 
	{
		int action = 1;
		negaMax(state,(turn==MAX) ? 1 : -1, &action, transpositionTable);
		
		//reset transposition table
		for (int i = 0; i < 200; i++)
		{
			transpositionTable[i] = 0;
		}
		
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
