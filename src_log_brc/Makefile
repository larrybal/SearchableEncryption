CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = log-brc
SOURCES = log-brc.cpp main.cpp
HEADERS = log-brc.hpp

$(TARGET): $(SOURCES) $(HEADERS)
		$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
		rm -f $(TARGET)