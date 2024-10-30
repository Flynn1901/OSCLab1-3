all : dplist.c dplist.h program.c
	mkdir -p build
	gcc dplist.c program.c -o build/dplistprogram -std=c11 -Wall -g

test: dplist.c dplist.h test.c
	gcc -g -Wall dplist.c test.c -o test

clean:
	rm -fr *.o build/*
	rm -r build

