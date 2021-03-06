PROJECT=7drl
CXX=g++
CXXFLAGS=-O2 -march=native -m32 -std=c++11
CXXLIBS=-lncurses

HEADERS=object.h screen.h map.h clock.h player.h action.h error.h
OBJECTS=entry.o screen.o map.o clock.o player.o config.o action.o error.o
SOURCE=$(subst .o,.cpp,$(OBJECTS))


.PHONY: clean run

all: $(PROJECT)

clean:
	rm -vf $(PROJECT)
	rm -vf *.o

run: $(PROJECT)
	./$(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

%.o: $(HEADERS)
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

