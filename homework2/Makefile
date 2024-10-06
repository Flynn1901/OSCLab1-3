all: hellomain.c hellofunc.c hellofunc.h
	gcc -o hello hellomain.c hellofunc.c

stepwise: hellomain.o hellofunc.o
	gcc -o hello hellomain.o hellofunc.o

hellomain.o: hellomain.c
	gcc -c hellomain.c

hellofunc.o: hellofunc.c hellofunc.h
	gcc -c hellofunc.c

clean: 
	rm -rf *.o
