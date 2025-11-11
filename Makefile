all:
	gcc -Wall -c *.c
	gcc -o sig-burger *.o

clean:
	rm -f *.o  sig-burger