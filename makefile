CC=gcc
CFLAGS=-c -Wall -ansi -g
LDFLAGS=
SOURCES=source/Graph.c source/Node.c source/Main.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=asa

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
$(EXECUTABLE)
