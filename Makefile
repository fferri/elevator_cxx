CXXFLAGS=-std=c++11 -g

.PHONY: clean

test: TestElevatorState.o ElevatorState.o PreconditionException.o
	$(CXX) -g $^ -o $@

clean:
	rm -f test *.o

