CXX = g++ -c
LD = g++
DEBUG = -g -Wall
FLAGS = -std=c++11 -lpthread

%.o: %.cpp
	$(CXX) $(DEBUG) -o $*.o $<  $(FLAGS)

all: run

run: MyThread.o TestThread.o
	$(LD) -o run $^ $(FLAGS)

clean:
	find . -name "*.o" -exec rm '{}' \;
	rm run
