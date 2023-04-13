prog: main.o
	g++ -g -Wall -std=c++11 -fopenmp main.o -o EXE

main.o: main.cpp Hash.h ChainingHash.h ProbingHash.h
	g++ -c -g -Wall -std=c++11 -fopenmp main.cpp

clean:
	rm *.o

run:
	@./EXE