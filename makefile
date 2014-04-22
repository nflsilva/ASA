CC=gcc
FILE =src/ASA_PROJ2_AL037.c
CFLAGS=-c -Wall -ansi -g
MOOFLAGS=-c -o3 -Wall -ansi -g $(FILE) -lm
LDFLAGS= 
SOURCES=src/*
TESTS=test/
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=asa

all: $(SOURCES) $(EXECUTABLE)

compile-mooshak:
	$(CC) $(MOOFLAGS)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
run-tests: test1 test2 test3 test4 test5

test1:
	$(EXECUTABLE) < $(TESTS)/01/input > $(TESTS)/01/rout
test2:
	$(EXECUTABLE) < $(TESTS)/02/input > $(TESTS)/02/rout
test3:
	$(EXECUTABLE) < $(TESTS)/03/input > $(TESTS)/03/rout
test4:
	$(EXECUTABLE) < $(TESTS)/04/input > $(TESTS)/04/rout
test5:
	$(EXECUTABLE) < $(TESTS)/05/input > $(TESTS)/05/rout
