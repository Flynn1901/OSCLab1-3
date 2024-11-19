all: main.c sensor_db.h sensor_db.c logger.h logger.c
	gcc -g -Wall main.c sensor_db.c -o main.out

run: all
	./main.out
log: logger.c logger.h
	gcc -g -Wall logger.c -o logger

zip:
	zip milestone2.zip *.c *.h Makefile
