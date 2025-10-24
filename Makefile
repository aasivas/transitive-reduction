CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -fopenmp

all: test

test: tests/test_transitive_reduction.cpp
	$(CXX) $(CXXFLAGS) -o test_runner tests/test_transitive_reduction.cpp

run_test:
	./test_runner

clean:
	rm -f test_runner
