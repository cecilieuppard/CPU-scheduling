CC=gcc
CFLAGS= -I -Wall -w

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

scheduler: red_black_tree.o stack.o misc.o utils.o CFS.o FCFS.o scheduler.o
	$(CC) -o scheduler red_black_tree.o stack.o misc.o utils.o CFS.o FCFS.o scheduler.o -I.

clean:
	rm -f *.o *~ core* scheduler
