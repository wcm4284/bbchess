SHELL := bash

B := build
COMPILER := g++
FLAGS := -O2 -Wall -Werror -Wpedantic -g -std=c++20


FILES := main.cpp
OBJECTS := $(patsubst %.cpp,$B/%.o,$(FILES))

main_engine: $(OBJECTS)
	@echo "Linking $@"
	$(COMPILER) -o $B/$@ $(OBJECTS) $(FLAGS)


$B/%.o : %.cpp Makefile
	@echo "compiling $<"
	@mkdir -p $(B)
	$(COMPILER) -I. -c -MD -MF $B/$*.d -o $B/$*.o $< $(FLAGS)


all: $(FILES)

clean:
	@rm -r build

run: main_engine
	@$B/./main_engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
