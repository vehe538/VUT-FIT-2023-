# Makefile for test/helloworld.cpp (SIMLIB examples)

CXX ?= g++
# Do not reference ../src — build only using headers/libraries available
# in the test repository or installed system locations (e.g. /usr/local/include).
CXXFLAGS ?= -std=c++11 -O2 -Wall

# Always build against the system-installed libsimlib (headers in
# /usr/local/include, library in /usr/local/lib). Use rpath so the
# executable can find the shared library at runtime.
LDLIBS := -lsimlib -lm

TARGET = main
SRC = $(TARGET).cpp

.PHONY: all clean run
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) 

