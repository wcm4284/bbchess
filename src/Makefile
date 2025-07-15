SHELL := bash

B := ../build
COMPILER := g++

DEBUG ?= 0

ifeq ($(DEBUG), 1)
FLAGS := -O0 -g -Wall -Wextra -Wpedantic -std=c++20 -mbmi2
else
FLAGS := -O2 -g -Wall -Wextra -Wpedantic -std=c++20 -mbmi2
endif


FILES := bitboard.cpp position.cpp movegen.cpp search.cpp evaluate.cpp main.cpp moveorder.cpp thread.cpp engine.cpp uciengine.cpp
OBJECTS := $(patsubst %.cpp,$B/%.o,$(FILES))

$B/main_engine: $(OBJECTS)
	@echo "Linking $@"
	@$(COMPILER) -o $@ $(OBJECTS) $(FLAGS)
	@ln -sf $@ engine



$B/%.o : %.cpp Makefile
	@echo "compiling $<"
	@mkdir -p $(B)
	$(COMPILER) -I. -c -MD -MF $B/$*.d -o $B/$*.o $< $(FLAGS) -DUSE_PEXT

all: $B/main_engine

debug:
	@make DEBUG=1

clean:
	@rm -r $B
	@rm engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
