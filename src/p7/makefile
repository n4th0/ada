# Nathan Rodriguez Moyses
OPTS = -Wall -Wextra -O3 -std=c++0x
CC = g++

all: maze_greedy


run: maze_greedy
	@./maze_greedy $(args)


maze_greedy: maze_greedy.cc
	@$(CC) ${OPTS} -o maze_greedy maze_greedy.cc

tar: maze_greedy.cc makefile
	tar cvfz 48727425Q.tar.gz maze_greedy.cc makefile

clean: 
	rm -f  48727425Q.tar.gz maze_greedy *~
 
