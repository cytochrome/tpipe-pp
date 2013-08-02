
PREFIX   := /usr/local

CXX      := g++
OUT_NAME := tpipe
CXXFLAGS := -Wall -ansi -pedantic -O2
LDFLAGS  := -lstdc++ -lboost_program_options
LDFLAGS_S := -lstlport -lboost_program_options -static

OBJS     := tpipe.o

.SUFFIXES: .cxx

all: dynamic

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

static: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS_S) -o $(OUT_NAME)-static

dynamic: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(OUT_NAME)

clean:
	rm -f $(OBJS) $(OUT_NAME)

install: all
	cp -f $OUT_NAME $(PREFIX)/bin/$OUT_NAME

