# Nathan Rodriguez Moyses
OPTS = -Wall -Wextra -O3 -std=c++0x
CC = g++

all: maze_bt


run: maze_bt
	@./maze_bt $(args)


maze_bt: maze_bt.cc
	@$(CC) ${OPTS} -o maze_bt maze_bt.cc

tar: maze_bt.cc makefile
	tar cvfz 48727425Q.tar.gz maze_bt.cc makefile

clean: 
	rm -f  48727425Q.tar.gz maze_bt *~
 
