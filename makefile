CC = gcc

CFLAGS = -c -g -I/home/$(USER)/local/include -I/usr/include/
LFLAGS = -lm -L/home/$(USER)/local/lib -Wl,-R /home/$(USER)/local/lib

grid2:
	echo im compiling $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o
	$(CC) $@.o $(LFLAGS) -lgsl -lgslcblas -lm -o $@.x

clean:
	rm -rf $(PROGRAM)
	rm -rf *~
	rm -rf *.out
	rm -rf *#
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm *.x
