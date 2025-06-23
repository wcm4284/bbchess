SHELL := bash

B := ../build
COMPILER := g++

DEBUG ?= 0

ifeq ($(DEBUG),1)
	FLAGS := -O0 -g -Wall -Wextra -Wpedantic -std=c++20 -mbmi2 -DDEBUG
else
	FLAGS := -O2 -Wall -Wextra -Wpedantic -std=c++20 -mbmi2
endif

FILES := main.cpp bitboard.cpp position.cpp movegen.cpp
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

debug:
	make DEBUG=1

run: $B/main_engine
	@$B/main_engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
