CC=gcc
CFLAGS= -std=c99 -Wall -g

knightdistance:	queuesPos.c astar.c
		$(CC) $(CFLAGS) queuesPos.c astar.c -lm -o $@
	
clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
