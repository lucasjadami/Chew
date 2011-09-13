FLAGS=-g -D DEBUG_PRINT -lncurses
CC=g++

all: chew

chew: main.o parser.o command.o iohandler.o runner.o dirhandler.o
	$(CC) $(FLAGS) main.o parser.o command.o iohandler.o runner.o dirhandler.o -o chew

main.o: main.cpp
	$(CC) -c $(FLAGS) main.cpp
	
parser.o: parser.cpp
	$(CC) -c $(FLAGS) parser.cpp
	
command.o: command.cpp
	$(CC) -c $(FLAGS) command.cpp
	
iohandler.o: iohandler.cpp
	$(CC) -c $(FLAGS) iohandler.cpp
	
runner.o: runner.cpp
	$(CC) -c $(FLAGS) runner.cpp
	
dirhandler.o: dirhandler.cpp
	$(CC) -c $(FLAGS) dirhandler.cpp
	
clean:
	rm -rf *o chew
