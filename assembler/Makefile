INCLUDE = ../include
OBJ     = obj/
SRC     = src/

CXX = g++ -I $(INCLUDE) $(CFLAGS)

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(foreach x,$(SOURCES), $(OBJ)/$(x).o)

all: asm

run: asm
	./asm $(ARGS)

asm: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

$(OBJ)/%.o: %
	mkdir -p $(dir $@)
	$(CXX) -o $@ -c $<

clean:
	rm -rf $(OBJ)/$(OBJECTS) proc