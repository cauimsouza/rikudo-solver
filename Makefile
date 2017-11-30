CXX=g++
CFLAGS=-std=c++11 -c
LDFLAGS=-lcryptominisat5
SOURCES=main.cpp graph.cpp
DEPS=graph.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rikudo

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: $(SOURCES)
	$(CXX) $(CFLAGS) -c $< 

clean:
	rm $(OBJECTS) $(EXECUTABLE)

