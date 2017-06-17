CC=g++
CFLAGS=-I./src -std=c++11
SOURCES = $(wildcard src/*.cpp src/trackerml/*.cpp)
#OBJECTS=$(SOURCES:.cpp=.o)
#DEPS = ./src/trackerml/SAXHandler.h
EXECUTABLE=tracker-ml

all: 
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

#all: $(SOURCES) $(EXECUTABLE)

#$(EXECUTABLE): $(OBJECTS) 
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@
