CC=g++
CFLAGS=-Wall -std=c++11 -g
LFLAGS=-lgtest

all:
	$(CC) $(CFLAGS) tests.cpp -o tests.o $(LFLAGS)
