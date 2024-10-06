CXX = g++
CXXFLAGS = -Wall -Wextra #-std=c++11

SRCS = $(wildcard src/*.cpp)

TARGET = guessr

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean