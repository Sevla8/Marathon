OPTIONS=-O2 -Wall

all : tp3

tp3 : tp3.cpp pointst.o carte.o fcarte.o
	g++ ${OPTIONS} -o tp3 tp3.cpp pointst.o carte.o fcarte.o

pointst.o : pointst.cpp pointst.h
	g++ ${OPTIONS} -c -o pointst.o pointst.cpp

carte.o : carte.cpp carte.h pointst.h
	g++ ${OPTIONS} -c -o carte.o carte.cpp

fcarte.o : fcarte.cpp carte.h
	g++ ${OPTIONS} -c -o fcarte.o fcarte.cpp

clean:
	rm -rf tp3 chemin *~ *.o
