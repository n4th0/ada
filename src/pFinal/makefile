# Nathan Rodriguez Moyses
OPTS = -Wall -Wextra -O3 -std=c++17
CC = g++

all: maze_bb


run: maze_bb
	@./maze_bb $(args)


maze_bb: maze_bb.cc
	@$(CC) ${OPTS} -o maze_bb maze_bb.cc

tar: maze_bb.cc makefile
	tar cvfz 48727425Q.tar.gz maze_bb.cc makefile memoria.pdf

clean: 
	rm -f  48727425Q.tar.gz maze_bb *~
 
