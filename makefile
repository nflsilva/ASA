CC=gcc
FILE =src/ASA_PROJ2_AL037.c
CFLAGS=-g
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
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
run-tests: test1 test2 test3 test4 test5

test1:
	./$(EXECUTABLE) < $(TESTS)/01/input > $(TESTS)/01/rout
test2:
	./$(EXECUTABLE) < $(TESTS)/02/input > $(TESTS)/02/rout
test3:
	./$(EXECUTABLE) < $(TESTS)/03/input > $(TESTS)/03/rout
test4:
	./$(EXECUTABLE) < $(TESTS)/04/input > $(TESTS)/04/rout
test5:
	./$(EXECUTABLE) < $(TESTS)/05/input > $(TESTS)/05/rout
	
extra:
	./$(EXECUTABLE) < test-s/Input01.in > test-s/Output01
	./$(EXECUTABLE) < test-s/Input02.in > test-s/Output02
	./$(EXECUTABLE) < test-s/Input03.in > test-s/Output03
	./$(EXECUTABLE) < test-s/Input04.in > test-s/Output04
	./$(EXECUTABLE) < test-s/Input05.in > test-s/Output05
	./$(EXECUTABLE) < test-s/Input06.in > test-s/Output06
	./$(EXECUTABLE) < test-s/Input07.in > test-s/Output07
	./$(EXECUTABLE) < test-s/Input08.in > test-s/Output08
	./$(EXECUTABLE) < test-s/Input09.in > test-s/Output09
	./$(EXECUTABLE) < test-s/Input10.in > test-s/Output10
	./$(EXECUTABLE) < test-s/Input11.in > test-s/Output11
	./$(EXECUTABLE) < test-s/Input12.in > test-s/Output12
	./$(EXECUTABLE) < test-s/Input13.in > test-s/Output13
	./$(EXECUTABLE) < test-s/Input14.in > test-s/Output14
	./$(EXECUTABLE) < test-s/Input15.in > test-s/Output15
	./$(EXECUTABLE) < test-s/Input16.in > test-s/Output16
	./$(EXECUTABLE) < test-s/Input17.in > test-s/Output17
	./$(EXECUTABLE) < test-s/Input18.in > test-s/Output18
	./$(EXECUTABLE) < test-s/Input19.in > test-s/Output19
	./$(EXECUTABLE) < test-s/Input20.in > test-s/Output20
	
diff:
	diff test-s/Input01.expected test-s/Output01
	diff test-s/Input02.expected test-s/Output02
	diff test-s/Input03.expected test-s/Output03
	diff test-s/Input04.expected test-s/Output04
	diff test-s/Input05.expected test-s/Output05
	diff test-s/Input06.expected test-s/Output06
	diff test-s/Input07.expected test-s/Output07
	diff test-s/Input08.expected test-s/Output08
	diff test-s/Input09.expected test-s/Output09
	diff test-s/Input10.expected test-s/Output10
	diff test-s/Input11.expected test-s/Output11
	diff test-s/Input12.expected test-s/Output12
	diff test-s/Input13.expected test-s/Output13
	diff test-s/Input14.expected test-s/Output14
	diff test-s/Input15.expected test-s/Output15
	diff test-s/Input16.expected test-s/Output16
	diff test-s/Input17.expected test-s/Output17
	diff test-s/Input18.expected test-s/Output18
	diff test-s/Input19.expected test-s/Output19
	diff test-s/Input20.expected test-s/Output20	
