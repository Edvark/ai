#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "fringe.h"

Fringe makeFringe(int mode) {
  /* Returns an empty fringe. 
   * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP) 
   */
  Fringe f;
  if ((mode != LIFO) && (mode != STACK) && (mode != FIFO) &&
      (mode != PRIO) && (mode != HEAP)) {
    fprintf(stderr, "makeFringe(mode=%d): incorrect mode. ", mode);
    fprintf(stderr, "(mode <- [LIFO,STACK,FIFO,PRIO,HEAP])\n");
    exit(EXIT_FAILURE);
  }
  f.mode = mode;
  f.size = f.front = f.rear = 0; /* front+rear only used in FIFO mode */
  f.states = malloc(MAXF*sizeof(State));
  if (f.states == NULL) {
	fprintf(stderr, "makeFringe(): memory allocation failed.\n");
    exit(EXIT_FAILURE);      
  }
  f.maxSize = f.insertCnt = f.deleteCnt = 0;
  return f;
}

void deallocFringe(Fringe fringe) {
  /* Frees the memory allocated for the fringe */
  free(fringe.states);
}

int getFringeSize(Fringe fringe) {
  /* Returns the number of elements in the fringe 
   */
  return fringe.size;
}

int isEmptyFringe(Fringe fringe) {
  /* Returns 1 if the fringe is empty, otherwise 0 */
  return (fringe.size == 0 ? 1 : 0);
}

Fringe insertFringe(Fringe fringe, State s) {
  /* Inserts s in the fringe, and returns the new fringe.
   * This function needs a third parameter in PRIO(HEAP) mode.
   */

  if (fringe.size == MAXF) {
    fprintf(stderr, "insertFringe(..): fatal error, out of memory.\n");
    exit(EXIT_FAILURE);    
  }
  fringe.insertCnt++;
  switch (fringe.mode) {
	  case LIFO: /* LIFO == STACK */
	  case STACK:
		fringe.states[fringe.size] = s;
		break;
	  case FIFO:
		fringe.states[fringe.rear++] = s;
		fringe.rear %= MAXF;
		break;
	  case PRIO: /* PRIO == HEAP */
	  case HEAP:
	  {
		//insert at first free position in the heap
		int currentIndex = fringe.size;	
		currentIndex++;
		fringe.states[currentIndex] = s;
		//restore heap order with upheap
		while(currentIndex > 0 && fringe.states[currentIndex/2].cost > fringe.states[currentIndex].cost)
		{
			//swap
			State z = fringe.states[currentIndex/2];
			fringe.states[currentIndex/2] = fringe.states[currentIndex];
			fringe.states[currentIndex] = z;
			
			currentIndex = currentIndex/2;
		}
		break;
	  }
  }
  fringe.size++;
  if (fringe.size > fringe.maxSize) {
    fringe.maxSize = fringe.size;
  }
  return fringe;
}

Fringe removeFringe(Fringe fringe, State *s) {
  /* Removes an element from the fringe, and returns it in s. 
   * Moreover, the new fringe is returned.
   */
   
  if (fringe.size < 1) {
    fprintf(stderr, "removeFringe(..): fatal error, empty fringe.\n");
    exit(EXIT_FAILURE);    
  }
  fringe.deleteCnt++;
  fringe.size--;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
    *s = fringe.states[fringe.size];
    break;
  case FIFO:
    *s = fringe.states[fringe.front++];
    fringe.front %= MAXF;
    break;
  case PRIO: /* PRIO == HEAP */
  case HEAP:
	//get the root of the heap and swap it with the last node in the heap
    *s = fringe.states[1];
    fringe.states[1] = fringe.states[fringe.size+1];
    int currentIndex = 1;
    int maxIndex = 1;
    
    //restore heap order using downheap
    while(2*currentIndex+1 <= fringe.size)
    {
		if(fringe.states[currentIndex*2].cost < fringe.states[currentIndex].cost)
			maxIndex = 2*currentIndex;
		
		if(fringe.states[maxIndex].cost > fringe.states[2*currentIndex + 1].cost)
			maxIndex = 2* currentIndex + 1;
			
		if(currentIndex == maxIndex)
			break;
		
		//swap	
		State z = fringe.states[maxIndex];
		fringe.states[maxIndex] = fringe.states[currentIndex];
		fringe.states[currentIndex] = z;
		
		currentIndex = maxIndex;
	}
    break;
  }
  return fringe;
}

void showStats(Fringe fringe) {
  /* Shows fringe statistics */
  printf("#### fringe statistics:\n");
  printf(" #size        : %7d\n", fringe.size);
  printf(" #maximum size: %7d\n", fringe.maxSize);
  printf(" #insertions  : %7d\n", fringe.insertCnt);
  printf(" #deletions   : %7d\n", fringe.deleteCnt);
  printf("####\n"); 
}
