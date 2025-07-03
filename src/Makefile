SHELL := bash

B := ../build
COMPILER := g++

DEBUG ?= 0

FLAGS := -O0 -g -Wall -Wextra -Wpedantic -std=c++20 -mbmi2

FILES := bitboard.cpp position.cpp movegen.cpp search.cpp evaluate.cpp main.cpp moveorder.cpp
OBJECTS := $(patsubst %.cpp,$B/%.o,$(FILES))

$B/main_engine: $(OBJECTS)
	@echo "Linking $@"
	@$(COMPILER) -o $@ $(OBJECTS) $(FLAGS)


$B/%.o : %.cpp Makefile
	@echo "compiling $<"
	@mkdir -p $(B)
	@$(COMPILER) -I. -c -MD -MF $B/$*.d -o $B/$*.o $< $(FLAGS) -DUSE_PEXT

all: $(FILES)

clean:
	@rm -r $B

run: $B/main_engine
	@echo
	@$B/main_engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
