all:
	gcc -Wall -c headers/*.h                # compila os .h em .gch
	gcc -Wall -c modulos/*.c -I headers     # compila os .c em .o
	gcc -Wall -c *c -I headers
	gcc -o sig-burger *.o

clean:
	rm -f *.o  sig-burger headers/*.gch