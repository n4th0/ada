# Nathan Rodriguez Moyses 48727425Q
OPTS = -Wall -Wextra -O3 -std=c++0x

all: qs-vs-hs qs-vs-hs.Msteps graphs

graphs: qs-vs-hs.Msteps qs-vs-hs.gpi
	gnuplot qs-vs-hs.gpi

qs-vs-hs: qs-vs-hs.cc
	g++ ${OPTS} -o qs-vs-hs qs-vs-hs.cc	 

qs-vs-hs.Msteps: qs-vs-hs
	./qs-vs-hs >  qs-vs-hs.Msteps

tar: qs-vs-hs.cc qs-vs-hs.gpi makefile
	tar cvfz 48727425Q.tar.gz qs-vs-hs.cc qs-vs-hs.gpi makefile

clean: 
	rm -f qs-vs-hs qs-vs-hs.Msteps fit.log *.png *~
 
