# Makefile. 
#
# Use 'make' for general debug version.
# Use 'make dprint' for debug print version.
# Use 'make release' for release version.
# See below the flags for each target:

FLAGS=-Wall
CC=g++
	
all: debug

debug: FLAGS += -g -p
release: FLAGS += -s -O2
dprint: FLAGS += -D DEBUG_PRINT
dprint: debug

debug: chew
release: chew

chew: main.o parser.o command.o iohandler.o runner.o dirhandler.o jobshandler.o
	$(CC) $(FLAGS) main.o parser.o command.o iohandler.o runner.o dirhandler.o jobshandler.o -o chew

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
	
jobshandler.o: jobshandler.cpp
	$(CC) -c $(FLAGS) jobshandler.cpp
	
clean:
	rm -rf *o chew
	
test: test.cpp
	$(CC) $(FLAGS) -o test test.cpp
