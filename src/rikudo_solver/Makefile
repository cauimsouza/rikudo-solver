CXX=g++
CFLAGS=-std=c++11 -c 
SOURCES=main.cpp graph.cpp
DEPS=graph.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=RikudoSolver

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: $(SOURCES)
	$(CXX) $(CFLAGS) $< 

clean:
	rm $(OBJECTS) $(EXECUTABLE)

