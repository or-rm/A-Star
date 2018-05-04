IDIR =./include
CC=g++
CFLAGS=-std=c++11 -Wall -lm -fopenmp -I$(IDIR)

ODIR=./obj
LDIR =./lib
LFLAGS   = -Wall -lm

SRC=./src

_DEPS = nodestructures.h nodebinsearch.h nodequicksort.h readnodes.h readways.h astar.h astarqueues.h heuristic.h monitoring.h closestpoint.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_ULIBS = nodebinsearch.o nodequicksort.o astarqueues.o heuristic.o monitoring.o closestpoint.o


$(LDIR)/%.o: $(LDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


_OBJ = readnodes.o readways.o astar.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
ULIBS  = $(patsubst %,$(ODIR)/%,$(_ULIBS))

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(ULIBS)

main: main.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(ULIBS) $(OBJ) 	


.PHONY: clean

clean:
	rm -if $(ODIR)/*.o

