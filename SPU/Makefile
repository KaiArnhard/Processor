CXX = g++

INCLUDE = ../include
OBJ     = obj/
SRC     = src/

PROC  = ProcFunctions
STACK = StackFunctions
MAIN  = main

CFLAGS     = $(IFLAGS)
CFLAGS_OBJ = $(IFLAGS) -c
IFLAGS     = -I  $(INCLUDE)

all: proc

proc: $(OBJ)$(MAIN).o $(OBJ)$(PROC).o $(OBJ)$(STACK).o
	$(CXX) $(CFLAGS) $^ -o $@

$(MAIN).o: $(SRC)/$(MAIN).cpp 
	$(CXX) $(CFLAGS_OBJ) $< -o $(OBJ)$(MAIN).o

$(PROC).o: $(SRC)$(PROC).cpp
	$(CXX) $(CFLAGS_OBJ) $< -o $(OBJ)$(PROC).o

$(STACK).o: $(SRC)$(STACK).cpp
	$(CXX) $(CFLAGS_OBJ) $< -o $(OBJ)$(STACK).o

clean:
	rm -rf *.o proc
