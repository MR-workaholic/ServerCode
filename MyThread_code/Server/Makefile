CXX = g++ -c
LD = g++
DEBUG = -g -Wall
FLAGS = -std=c++11 -lpthread

%.o: %.cpp
	$(CXX) $(DEBUG) -o $*.o $<  $(FLAGS)

all: server

server: MyThread.o ThreadServer.o
	$(LD) -o server $^ $(FLAGS)

clean:
	find . -name "*.o" -exec rm '{}' \;
	rm server
