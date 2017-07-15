CC=g++
CFLAGS=-g -I./src -std=c++11 -Wall
SOURCES=$(wildcard src/*.cpp src/trackerml/*.cpp)
OBJECTS=$(patsubst src/%, obj/%, $(SOURCES:.cpp=.o))
HEADERS=$(SOURCES:.cpp=.h)
DEPS := $($(SOURCES:.cpp=.o):.o=.d)
TARGET=trackerml

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

obj/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf obj
	mkdir -p obj/trackerml
	
check:
	cppcheck --enable=all --suppress=missingIncludeSystem src/

format:
	for SRC in $(SOURCES);do if [ -f $$SRC ]; then clang-format -style=file -i "$$SRC"; fi; done
	for SRC in $(HEADERS);do if [ -f $$SRC ]; then clang-format -style=file -i "$$SRC"; fi; done
	
depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;

include .depend
