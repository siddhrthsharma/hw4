CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = ziptree
SRC = ziptree.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
