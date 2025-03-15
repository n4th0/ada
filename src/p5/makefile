# Nathan Rodriguez Moyses 48727425Q
OPTS = -Wall -Wextra -O3 -std=c++0x
CC = g++

all:  graphs

graphs: powSteps pow2.gpi
	@gnuplot pow2.gpi 


pow: pow2.cc
	$(CC) $(OPTS) pow2.cc -o pow2
	@# @./pow

powSteps: pow2
	@./pow2 >  powSteps

tar: pow2.cc pow2.gpi makefile
	tar cvfz 48727425Q.tar.gz pow2.cc pow2.gpi makefile

clean: 
	@rm -f pow2 powSteps *.png *~
 
