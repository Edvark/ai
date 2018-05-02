/* Name(s): Duncan Saunders & Edward Boere
*  Emails: duncansaunders1997@gmail.com & e.e.boere@student.rug.nl
* Program: mazeSolve.c
* Date: 23 March 2018
* Description: Takes a maze, number of runners, heights of grass minimum,
* and gates maximum, and works out the shortest time to complete path.
* Requires queuesPos.c to run*/



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queuesPos.h"

/*The function action produces the four different directions you can take*/
Position action(Position p, char** maze, int rows, int columns, int nextDirection, int height, int gateHeight, int flowerHeight) {

	/*if nextDirection==1 and the next position allows you to go there, you go one step down*/
	if (nextDirection==1 && (maze[p.xcoord+1][p.ycoord] == '.' || (maze[p.xcoord+1][p.ycoord]== '^' && height <= gateHeight) || (maze[p.xcoord+1][p.ycoord]== '%' && height >= flowerHeight))){
		p.time++;
		p.xcoord = p.xcoord+1;
	}
	/*if nextDirection==2 you go one step up*/
	if (nextDirection==2 && (maze[p.xcoord-1][p.ycoord] == '.' || (maze[p.xcoord-1][p.ycoord]== '^' && height <= gateHeight) || (maze[p.xcoord-1][p.ycoord]== '%' && height >= flowerHeight))){
		p.time++;
		p.xcoord = p.xcoord-1;
	}
	/*if nextDirection==3 you go one step to the right*/
	if (nextDirection==3 && (maze[p.xcoord][p.ycoord+1] == '.' || (maze[p.xcoord][p.ycoord+1]== '^' && height <= gateHeight) || (maze[p.xcoord][p.ycoord+1]== '%' && height >= flowerHeight))){
		p.time++;
		p.ycoord = p.ycoord+1;
	}
	/*if nextDirection==4 you go one step to the left*/
	if (nextDirection==4 && (maze[p.xcoord][p.ycoord-1] == '.' || (maze[p.xcoord][p.ycoord-1]== '^' && height <= gateHeight) || (maze[p.xcoord][p.ycoord-1]== '%' && height >= flowerHeight))){
		p.time++;
		p.ycoord = p.ycoord-1;
	}

	return p;
}

/* The function routes returns the fastest time for an individual to get outside of the maze */
int routes(char **maze, int height, int gateHeight, int flowerHeight, int rows, int columns) {
	Position p, new;
	int x, y;

	/*The start position*/
	p.time = 0;
	p.xcoord = rows/2;
	p.ycoord = columns/2;

	Queue q;
	q = newEmptyQueue();

	/*the array pos indicates the positions that already have been visited*/
	int *pos[rows];
	for (x = 0; x < rows; x++){
		pos[x] = malloc (columns*sizeof (int));
	}

	/*0 indicates that you haven't been there, 1 indicates that you have been there*/
	for (x=0 ; x<rows ; x++){
		for (y=0 ; y<columns ; y++){
			pos[x][y]=0;
		}
	}

	if (p.xcoord==0 || p.xcoord==rows-1 || p.ycoord==0 || p.ycoord==columns-1){ /*the sides of the maze are reached*/
		freeQueue(q);
		for(x=0 ; x<rows ; x++){
			free(pos[x]);
		}
		return p.time;
	}

	enqueue(p, &q);
	while (isEmptyQueue(q)!=1){
		p = dequeue(&q);
		if (pos[p.xcoord][p.ycoord]!=1){
			for (x=1 ; x<=4 ; x++){
				new = action(p, maze,  rows,  columns,  x,  height,  gateHeight, flowerHeight);
				if (new.xcoord==0 || new.xcoord==rows-1 || new.ycoord==0 || new.ycoord==columns-1){
					freeQueue(q);
					for(y=0 ; y<rows ; y++){
						free(pos[y]);
					}
					return new.time;
				}
				if (!(new.xcoord==p.xcoord && new.ycoord == p.ycoord)){
					enqueue(new, &q);
				}
			}
		}
		pos[p.xcoord][p.ycoord]=1;
	}
	freeQueue(q);

	for(x=0 ; x<rows ; x++){
		free(pos[x]);
	}

	return rows*columns;
}

/*The function shortestTime compares the fastest time for every person, to find the fastest time of the team*/
int shortestTime(int rows, int columns, int k, char** maze, int* persons, int gateHeight, int flowerHeight ){
	int time = rows*columns; /*longest time possible*/
	int a;

	/*find the fastest time for every individual*/
	for(a=0 ; a<3 ; a++){
		if (persons[a]!=-1){
			int	newTime = routes(maze, persons[a], gateHeight, flowerHeight, rows, columns);

			/*if a person has set a new record, replace the time*/
			if (time>newTime){
				time = newTime;
			}
		}
	}
	return time;
}

int main(int argc, char *argv[]) {

	int n = 0; /*n mazes*/
	int gateHeight, flowerHeight = 0; /*gates, flowers*/
	int k = 0; /*k persons*/
	int rows, columns = 0; /*row columns*/
	int a, i, j = 0; /*indices*/
	int man;

	scanf("%d", &n);

	/*for every hedgemaze: */
	for (a=0 ; a<n ; a++){
		scanf("%d %d", &gateHeight, &flowerHeight);
		scanf("%d", &k);
		/*the team can't be bigger than 99 persons*/
		if (k>=100){
			return 0;
		}

		/*There are three kinds of people*/
		int persons[3] = {-1, -1, -1};

		for (i=0 ; i<k ; i++){
			scanf("%d", &man);
			if (man<=gateHeight){
				persons[0] = man;
			}
			if (man>gateHeight || man<flowerHeight){
				persons[1] = man;
			}
			if (man>=flowerHeight){
				persons[2] = man;
			}
		}

		/*a small or tall person can do the same and more as a normal person, so you don't need the normal person*/
		if (persons[0]!= -1 || persons[2]!=-1){
			persons[1]=-1;
		}

		scanf("%d %d", &rows, &columns);

		/*there must be a middle, thus the rows and columns must be odd*/
		if (rows%2==0 || columns%2 ==0 || rows>=1000 || columns>=1000){
			return 0;
		}

		/*make the maze*/
		char *maze[rows];
		for (i = 0; i < rows; i++){
			maze[i] = malloc (columns * sizeof (char));
		}

		/*maze is put in 2D array*/
		for (i=0 ; i<rows ; i++){
			for (j=0 ; j<columns ; j++){
				char in;
				do{
					in = getchar();
					maze[i][j] = in;
				} while (in!='#' && in!='^' && in!='.' && in!='%');
			}
		}

		/*find the fastest time*/
		int time=shortestTime(rows, columns, k, maze, persons, gateHeight, flowerHeight);

		printf("%d\n", time+1);

		/*free arrays*/
		for(i=0 ; i<rows ; i++){
			free(maze[i]);
		}
	}
	return 0;
}
