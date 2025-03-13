# Nathan Rodriguez Moyses 48727425Q
OPTS = -Wall -Wextra -O3 -std=c++0x
CC = g++

all:  graphs

graphs: powSteps pow2.gpi
	powStepsgnuplot 


pow: pow2.cc
	@$(CC) $(OPTS) pow2.cc -o pow
	@# @./pow

powSteps: pow
	@./pow >  powSteps

tar: pow2.cc pow2.gpi makefile
	tar cvfz 48727425Q.tar.gz qs-vs-hs.cc qs-vs-hs.gpi makefile

clean: 
	rm -f qs-vs-hs qs-vs-hs.Msteps fit.log *.png *~
 
