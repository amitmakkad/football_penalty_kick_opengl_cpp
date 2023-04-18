CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp functionalities.cpp shapes.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: $(CC) $(CFLAGS) $< -o $@

clean: rm -f $(OBJECTS) $(EXECUTABLE)
