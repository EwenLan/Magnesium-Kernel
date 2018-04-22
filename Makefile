CXX = g++
INCLUDE = .
CPPFLAGS = -O -Wall --std=c++11
all: main
main: main.o ports.o
	$(CXX) main.o ports.o -lpthread -o main
main.o: main.cpp ports.hpp
	$(CXX) -I$(INCLUDE) -lpthread $(CPPFLAGS) -c main.cpp
ports.o: ports.cpp ports.hpp
	$(CXX) -I$(INCLUDE) $(CPPFLAGS) -c ports.cpp

clean:
	rm *.o -f
	rm main -f
	