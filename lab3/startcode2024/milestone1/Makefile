
ourtest : dplist.c ourtest.c
	mkdir -p build
	gcc -g dplist.c ourtest.c -o build/dplist_test
	./build/dplist_test

randomtest : dplist.c randomtest.c
	mkdir -p build
	gcc -g dplist.c randomtest.c -o build/dplist_test

all : dplist.c dplist_test.c
	mkdir -p build
	gcc -g dplist.c dplist_test.c -o build/dplist_test

test: dplist.c test.c
	gcc -g -Wall dplist.c test.c -o test

clean:
	rm -r build

zip:
	zip milestone1.zip *.c *.h Makefile
