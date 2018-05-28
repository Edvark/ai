/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100
#define MAXITERATIONS 5000
#define POPULATIONSIZE 1000
#define MUTATECHANCE 35

#define FALSE 0
#define TRUE  1
#define MAXSW 100

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

void initializeRandomGenerator() {
  /* this routine initializes the random generator. You are not
   * supposed to understand this code. You can simply use it.
   */
  time_t t;
  srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    queens[q] = (flag == 0? 0 : random()%nqueens); 
  }
}

/* returns TRUE if position (row0,column0) is in 
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /* on same row, */
  if (column0 == column1) return TRUE; /* column, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
  return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in 
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, queens[queen])) {
      if ((row != queen) || (col != queens[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print configuration on screen */
void printState() {
  int row, column; 
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (queens[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if ((column < 0) || (column >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: column=%d "
      "(should be 0<=column<%d)...Abort.\n", column, nqueens);
    exit(-1);
  }
  queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that 
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in canMoveTo: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if(column < 0 || column >= nqueens) return FALSE;
  if (queens[queen] == column) return FALSE; /* queen already there */
  return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in columnOfQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, queens[queen], other, queens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 1000
void randomSearch() {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;

  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int pos, newpos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      newpos = pos;
      while (newpos == pos) {
        newpos = random() % nqueens;
      }
      moveQueen(queen, newpos);
    }
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}

/*************************************************************/

void climb(int * minConflicts, int * startConflicts, int ** costMatrix)
{
	*minConflicts = countConflicts();
	*startConflicts = *minConflicts;
	int counter = 0;
	
	//for loop that puts the conflict values in the matrix
	for (int i = 0; i < nqueens; i++)
	{
		int currentPos = columnOfQueen(i);
		for (int j = 0; j < nqueens; j++)
		{
			//put a -1 at the position where we find a queen
			if(currentPos == j)
			{
				costMatrix[i][j] = -1;
			}
			else
			{
				queens[i] = j;
				costMatrix[i][j] = countConflicts();
				
				//we found a new lowest value
				if(costMatrix[i][j] < *minConflicts)
				{
					*minConflicts = costMatrix[i][j];
					counter = 1;
				}
				else if(costMatrix[i][j] == *minConflicts)
				{
					counter++;
				}
			}
		}
		//reset queens
		queens[i] = currentPos;

	}

	//select a random queen from the minimum values and move that queen
	counter = random() % (counter + 1);

	for (int i = 0; i < nqueens; i++)
	{
		for (int j = 0; j < nqueens; j++)
		{
			if(costMatrix[i][j] == *minConflicts)
			{
				if(counter == 0)
				{
					queens[i] = j;
				}
				counter--;
			}

			if(counter == -1)
			{
				break;
			}
		}
	}
}

//function that implements hill climbing
void hillClimbing() {
  int** costMatrix = malloc(nqueens*sizeof(int*));
  for (int i = 0; i < nqueens; i++)
  {
	  costMatrix[i] = malloc(nqueens*sizeof(int));
  }
  
  int minConflicts;
  int startConflicts;
  int sideways = MAXSW;
  
  do 
  {
	  climb(&minConflicts, &startConflicts, costMatrix);
	  
	  sideways = (minConflicts == startConflicts)? sideways - 1: MAXSW;
  } while(minConflicts < startConflicts || sideways > 0);
  
  //print solution and free everything
  printState();
  for (int i = 0; i < nqueens; i++)
  {
	  free(costMatrix[i]);
  }
  free(costMatrix);
}

/*************************************************************/
//function that maps the time to temperature
float timeToTemperature(int t) {
	float startTemp = 0.5;
	return startTemp / (float)log(t+1);
}

void simulatedAnnealing() {
	float temperature;
	int next;
	int deltaE;
	int newConflicts;
	int startConflicts;
	int currentPos;
	
	//initiliaze cost matrix
	int** costMatrix = malloc(nqueens*sizeof(int*));
	for (int i = 0; i < nqueens; i++)
	{
		costMatrix[i] = malloc(nqueens*sizeof(int));
	}

	int minConflicts;
  
	for (int i = 0; i <= MAXITERATIONS; i++)
	{
		//have a 50% chance it will actually choose the best option
		if(0.5 < (double)rand() / (double)RAND_MAX)
		{
			climb(&minConflicts, &startConflicts, costMatrix);
		}
		else
		{
			//current conflicts
			startConflicts = countConflicts();
			
			
			temperature = timeToTemperature(i);
			
			//solution found
			if(temperature == 0 || startConflicts == 0)
			{
				printState();
				for (int i = 0; i < nqueens; i++)
				{
					free(costMatrix[i]);
				}
				free(costMatrix);
				return;
			}
			
			do
			{
				next = random() % (nqueens * nqueens);
			}while(!canMoveTo(next / nqueens, next % nqueens));
			
			//next state conflicts
			currentPos = columnOfQueen(next / nqueens);
			queens[next/nqueens] = next % nqueens;		
			newConflicts = countConflicts();

			deltaE = startConflicts - newConflicts;
			//reset queen
			queens[next/nqueens] = currentPos;
			
			
			//determine if we go to the next state
			if(newConflicts <= startConflicts)			
			{
				queens[next/nqueens] = next % nqueens;
			}
			else if (exp(deltaE/temperature) > (double)rand() / (double)RAND_MAX)
			{
				queens[next/nqueens] = next % nqueens;
			}
		}
		
	}
	printState();
	for (int i = 0; i < nqueens; i++)
	{
		free(costMatrix[i]);
	}
	free(costMatrix);
}

/*************************************************************/
//fitness function based on the amount of non attacking pairs
int fitness(int * currentQueens)
{
	for (int i = 0; i < nqueens; i++)
	{
		queens[i] = currentQueens[i];
	}
	return nqueens * (nqueens - 1)/ 2 - countConflicts();
}

//create a child based of two parents
int * reproduce(int * x, int * y, int n)
{
	int *child = malloc(nqueens * sizeof(int));
	
	for (int i = 0; i < n; i++)
	{
		child[i] = x[i];
	}
	
	for (int i = n; i < nqueens; i++)
	{
		child[i] = y[i];
	}
	
	return child;
}

//select two parents and select the best out of the two based on fitness
int * randomSelection(int  * currentPopulation[POPULATIONSIZE], int totalCurrentPQ)
{
	int rand1 = random() % (nqueens*10);
	int rand2;
	do
	{
		rand2 = random() % (nqueens*10);
	}while(rand1 == rand2);
	
	if(fitness(currentPopulation[rand1]) > fitness(currentPopulation[rand2]))
	{
		return currentPopulation[rand1];
	}
	return currentPopulation[rand2];
}

//do a random mutation to the child
int * mutate(int * child)
{
	if(random() % 100 <= MUTATECHANCE)
	{
		int rand = random() % nqueens;
		child[rand] = random() % nqueens;
	}
	return child;
}

void genetic()
{
	int * newPopulation[POPULATIONSIZE];
	int * currentPopulation[POPULATIONSIZE];
	int totalNewPQ = 0;
	int totalCurrentPQ = 0;
	
	//initialize some random population
	for (int i = 0; i < nqueens*10; i++)
	{
		currentPopulation[i] = malloc(nqueens * sizeof(int));
		for (int j = 0; j < nqueens; j++)
		{
			currentPopulation[i][j] = random()%nqueens;
		}
		totalNewPQ += fitness(currentPopulation[i]);
	}
	
	
	for (int i = 0; i < MAXITERATIONS; i++)
	{
		totalCurrentPQ = totalNewPQ;
		totalNewPQ = 0;
		
		//go through the population
		for (int j = 0; j < nqueens*10/2; j++)
		{
			//set from two parents two new childs
			int * x;
			int * y;
			int * child1;
			int * child2;
			int n = random()%(nqueens - 2) + 1;
			x = randomSelection(currentPopulation, totalCurrentPQ);
			do
			{
				y = randomSelection(currentPopulation, totalCurrentPQ);
			}while(x == y);
			
			//reproduce
			child1 = reproduce(x, y, n);
			child2 = reproduce(y, x, n);
			
			//mutate
			mutate(child1);
			mutate(child2);
			
			if(i != 0)
			{
				free(newPopulation[2*j]);
				free(newPopulation[2*j+1]);
				
			}
			
			//set it in the new population and return it if they are the final solution
			newPopulation[2*j] = child1;
			newPopulation[2*j+1] = child2;
			
			if(fitness(child1) == nqueens * (nqueens - 1)/ 2)
			{
				for (int k = 0; k < nqueens; k++)
				{
					queens[k] = child1[k];
				}
				printState();	
				for (int i = 0; i < nqueens*10; i++)
				{
					free(currentPopulation[i]);
					free(newPopulation[i]);
				}
				return;
			}
			if(fitness(child2) == nqueens * (nqueens - 1)/ 2)
			{
				for (int k = 0; k < nqueens; k++)
				{
					queens[k] = child2[k];
				}
				printState();	
				for (int i = 0; i < nqueens*10; i++)
				{
					free(currentPopulation[i]);
					free(newPopulation[i]);
				}
				return;
			}
			totalNewPQ += fitness(child1) + fitness(child2);
			
			
			
		}
		//set currentPopulation to the new population
		for (int j = 0; j < nqueens*10; j++)
		{
			for (int k = 0; k < nqueens; k++)
			{
				currentPopulation[j][k] = newPopulation[j][k];
			}
			
		}	
	}
	//find the best one of the population
	int maxPQ = fitness(currentPopulation[0]);
	int index = 0;
	for (int i = 1; i < nqueens*10; i++)
	{
		int pq = fitness(currentPopulation[i]);
		
		if(pq > maxPQ)
		{
			maxPQ = pq;
			index = i;
		}
	}
	
	//copy the best solution
	for (int i = 0; i < nqueens; i++)
	{
		queens[i] = currentPopulation[index][i];
	}
	
	//print solution and free everything
	printState();
	for (int i = 0; i < nqueens*10; i++)
	{
		free(currentPopulation[i]);
		free(newPopulation[i]);
	}
}

int main(int argc, char *argv[]) 
{
  srand ((unsigned int)time(NULL));
  int algorithm;

  do {
    printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
    scanf ("%d", &nqueens);
  } while ((nqueens < 1) || (nqueens > MAXQ));

  do {
    printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
    printf ("(3) Simulated Annealing (4) Genetic: ");
    scanf ("%d", &algorithm);
  } while ((algorithm < 1) || (algorithm > 4));
  
  initializeRandomGenerator();

  initiateQueens(1);
  
  printf("\nInitial state:");
  printState();
	int succes;
  switch (algorithm) 
  {
	  case 1: randomSearch();       break;
	  case 2: hillClimbing();       break;
	  case 3: simulatedAnnealing(); break;
	  case 4: genetic(); break;
  }

  return 0;  
}
