SHELL := bash

B := Build
COMPILER := g++
FLAGS := -O2 -Wall -Werror -Wpedantic -g -std=c++20 -mbmi2


FILES := main.cpp bitboard.cpp position.cpp
OBJECTS := $(patsubst %.cpp,$B/%.o,$(FILES))

$B/main_engine: $(OBJECTS)
	@echo "Linking $@"
	$(COMPILER) -o $@ $(OBJECTS) $(FLAGS)


$B/%.o : %.cpp Makefile
	@echo "compiling $<"
	@mkdir -p $(B)
	$(COMPILER) -I. -c -MD -MF $B/$*.d -o $B/$*.o $< $(FLAGS) -DUSE_PEXT


all: $(FILES)

clean:
	@rm -r $B

run: $B/main_engine
	@$B/main_engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
