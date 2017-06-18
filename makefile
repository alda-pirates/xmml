CC=g++
CFLAGS=-I./src -std=c++11 -Wall
SOURCES=$(wildcard src/*.cpp src/trackerml/*.cpp)
OBJECTS=$(patsubst src/%, obj/%, $(SOURCES:.cpp=.o))
#DEPS = ./src/trackerml/SAXHandler.h
TARGET=trackerml

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

obj/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf obj
	mkdir -p obj/trackerml
