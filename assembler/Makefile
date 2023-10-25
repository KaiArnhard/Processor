CXX = g++

INCLUDE = ../include
OBJ     = obj/
SRC     = src/

ASM    = AsmFunctions
INPUT  = input
MAIN   = main
LABELS = labels


CFLAGS     = $(IFLAGS)
CFLAGS_OBJ = $(IFLAGS) -c
IFLAGS     = -I  $(INCLUDE)

all: asm

asm: $(OBJ)$(MAIN).o $(OBJ)$(ASM).o $(OBJ)$(INPUT).o $(OBJ)$(LABELS).o
	$(CXX) $(CFLAGS) $^ -o $@

$(MAIN).o: $(SRC)$(MAIN).cpp
	$(CXX) $(CFLAGS_OBJ) $< -o $(OBJ)$(MAIN).o

$(ASM).o: $(SRC)$(ASM).cpp
	$(CXX) $(CFLAGS_OBJ) $< -o$(OBJ)$(ASM).o

$(INPUT).o: $(SRC)$(INPUT).cpp
	$(CXX) $(CFLAGS_OBJ) $< -o $(OBJ)$(INPUT).o

$(LABELS).o: $(SRC)$(LABELS).cpp
	$(CXX) $(CFLAGS_OBJ) $< -o $(OBJ)$(LABELS).o

run:
	./asm ../asm.txt ../cm.bin

clean:
	rm -rf *.o proc
