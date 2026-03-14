CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude
LDFLAGS  :=

SRCDIR   := src
SOURCES  := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS  := $(SOURCES:.cpp=.o)
TARGET   := game

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run:
	./$(TARGET)