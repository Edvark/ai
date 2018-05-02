/* queues.h, 24 March 2016 */

#ifndef QUEUESPOS_H
#define QUEUESPOS_H

/* First the type definitions. */

typedef struct Position { /* a position contains a time and the coordinates */
	int time;
	int xcoord, ycoord;
} Position;

typedef struct ListNode *List;  /* List is the type of lists of positions */

struct ListNode {
	Position item;
	List next;
};

typedef struct Queue { /* a queue is a list and a pointer to the last node */
	List list;
	List lastNode;
} Queue;

/* Now the declaration of the functions that are defined in queues.c
 * and are to be used outside it, e.g. in hedge.c */

Queue newEmptyQueue ();
void freeQueue (Queue q);
int isEmptyQueue (Queue q);
void enqueue (Position s, Queue *qp);
Position dequeue(Queue *qp);
List addItem(Position s, List li);

#endif
