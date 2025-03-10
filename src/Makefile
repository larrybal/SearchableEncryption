# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Source files
SRC = pibas.cpp encryption.cpp main.cpp

# Header files (optional, but included for clarity)
HDR = pibas.hpp encryption.hpp

# Output executable
OUT = main

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target: build the executable
all: $(OUT)

# Rule to link object files into the final executable
$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(OBJ)

# Rule to compile each .cpp file to .o file
%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -f $(OBJ) $(OUT)

# Rule for re-building everything (including header files)
rebuild: clean all
