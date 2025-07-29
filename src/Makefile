SHELL := bash

B := ../build
COMPILER := g++

DEBUG ?= 0

CXXFLAGS := -g -Wall -Wcast-qual -Wextra -pedantic -Wnull-dereference -Wshadow \
            -Wmissing-declarations -std=c++20 -mbmi2

ifeq ($(DEBUG),0)
    CXXFLAGS += -O2
endif


FILES := bitboard.cpp position.cpp movegen.cpp moveorder.cpp \
         evaluate.cpp tt.cpp search.cpp thread.cpp engine.cpp \
         uciengine.cpp main.cpp
OBJECTS := $(patsubst %.cpp,$B/%.o,$(FILES))

$B/main_engine: $(OBJECTS)
    @echo "Linking $@"
    @$(COMPILER) -o $@ $(OBJECTS) $(CXXFLAGS)
    @ln -sf $@ engine



$B/%.o : %.cpp Makefile
    @echo "compiling $<"
    @mkdir -p $(B)
    $(COMPILER) -I. -c -MD -MF $B/$*.d -o $B/$*.o $< $(CXXFLAGS) -DUSE_PEXT

all: $B/main_engine

debug:
    @make DEBUG=1

clean:
    @rm -r $B
    @rm engine

-include $(patsubst %.cpp,$B/%.d,$(FILES))
