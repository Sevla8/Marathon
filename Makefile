OPTIONS=-O2 -Wall

all : tp3

tp3 : tp3.cpp pointst.o carte.o graphe.h
	g++ ${OPTIONS} -o tp3 tp3.cpp pointst.o carte.o

pointst.o : pointst.cpp pointst.h
	g++ ${OPTIONS} -c -o pointst.o pointst.cpp

carte.o : carte.cpp carte.h graphe.h
	g++ ${OPTIONS} -c -o carte.o carte.cpp

clean:
	rm -rf tp3 chemin *~ *.o
