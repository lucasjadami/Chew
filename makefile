FLAGS=-g -D DEBUG_PRINT
CC=g++

all: chew

chew: main.o parser.o command.o
	$(CC) $(FLAGS) main.o parser.o command.o -o chew

main.o: main.cpp
	$(CC) -c $(FLAGS) main.cpp
	
parser.o: parser.cpp
	$(CC) -c $(FLAGS) parser.cpp
	
command.o: command.cpp
	$(CC) -c $(FLAGS) command.cpp
	
clean:
	rm -rf *o chew
