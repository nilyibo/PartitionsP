all: partitionsP

partitionsP: main.cpp
	g++ -O3 -fopenmp -o partitionsP main.cpp

clean:
	rm -rf partitionsP
