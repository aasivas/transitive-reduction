CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -fopenmp

all: test

test: tests/test_transitive_reduction.cpp
	$(CXX) $(CXXFLAGS) -o test_runner tests/test_transitive_reduction.cpp

test_bcsstk03: tests/test_bcsstk03.cpp
	$(CXX) $(CXXFLAGS) -o test_bcsstk03_runner tests/test_bcsstk03.cpp

run_test: test
	./test_runner

run_test_bcsstk03: test_bcsstk03
	./test_bcsstk03_runner

clean:
	rm -f test_runner
