CFLAGS=-m32 -c -Wall -D_SERVER -static
LDFLAGS=-lpthread -static
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=libraknet.a

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	ar -rv $@ $(OBJECTS)

.cpp.o:
	g++ $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
