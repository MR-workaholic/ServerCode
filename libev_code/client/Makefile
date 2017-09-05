CXX = g++ -c
LD = g++
DEBUG = -g -Wall
FLAGS = -std=c++11 -L/usr/local/lib /usr/local/lib/libev.a /usr/local/lib/libev.so -lev

%.o: %.cpp
	$(CXX) $(DEBUG) -o $*.o $<  $(FLAGS)

all: client

client: MyClient.o Method.o main.o
	$(LD) -o client $^ $(FLAGS)

clean:
	find . -name "*.o" -exec rm '{}' \;
	rm client
