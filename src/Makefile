SHELL := bash

B := build
COMPILER := g++
FLAGS := -O2 -Wall -Werror -Wpedantic -g -std=c++20


FILES := main.cpp
OBJECTS := $(patsubst %.cpp,$B/%.o,$(FILES))

main_engine: $(OBJECTS)
	@echo "Linking $@"
	$(COMPILER) -o $@ $(OBJECTS) $(FLAGS)


$B/%.o : %.cpp Makefile
	@echo "compiling $<"
	@mkdir -p $(B)
	$(COMPILER) -I. -c -MD -MF $B/$*.d -o $B/$*.o $< $(FLAGS)

	

all: $(FILES)

clean:
	@rm -r build main_engine

run: main_engine
	@./main_engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
