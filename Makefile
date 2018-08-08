CXX = g++
INCLUDE = .
CPPFLAGS = -O -Wall --std=c++17
all: main
main: main.o ports.o simulator
	$(CXX) main.o ports.o -lpthread -o main
main.o: main.cpp ports.hpp
	$(CXX) -I$(INCLUDE) -lpthread $(CPPFLAGS) -c main.cpp
ports.o: ports.cpp ports.hpp
	$(CXX) -I$(INCLUDE) $(CPPFLAGS) -c ports.cpp
simulator: Simulator.cpp
	$(CXX) Simulator.cpp -lpthread $(CPPFLAGS) -o simulator
clean:
	rm *.o -f
	rm main -f
	rm simulator -f
	