
CXXFLAGS=-std=c++2a -O0

all: core.o value.o

clean:
	rm -f core.o value.o

core.o: core.cpp
	g++ -c $(CXXFLAGS) $^ -o $@

value.o: value.cpp
	g++ -c $(CXXFLAGS) $^ -o $@
