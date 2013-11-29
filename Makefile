all: partitionsP combine

partitionsP: main.cpp
	g++ -O3 -fopenmp -o partitionsP main.cpp

combine: combine.cpp
	g++ -o combine combine.cpp

clean:
	rm -rf partitionsP combine
