# Nathan Rodriguez Moyses
OPTS = -Wall -Wextra -O3 -std=c++0x -g
CC = g++

all: maze


run: maze
	@./maze $(args)


maze: maze.cc
	@$(CC) ${OPTS} -o maze maze.cc

tar: maze.cc makefile
	tar cvfz 48727425Q.tar.gz maze.cc makefile

clean: 
	rm -f  48727425Q.tar.gz maze *~
 
