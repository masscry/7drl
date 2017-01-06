PROJECT=7drl
CXX=g++
CXXFLAGS=-g3 -O2 -march=native -m32 -std=c++11
CXXLIBS=-lncurses

OBJECTS=entry.o
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

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

