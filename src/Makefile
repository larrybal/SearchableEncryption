# Compiler settings
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2

# OpenSSL paths (adjust these paths if necessary)
OPENSSL_CFLAGS = $(shell pkg-config --cflags openssl)
OPENSSL_LDFLAGS = $(shell pkg-config --libs openssl)

# Source files
SRC = pibas.cpp encryption.cpp main.cpp

# Header files (optional, but included for clarity)
HDR = pibas.hpp encryption.hpp

# Output executable
OUT = main

# Object files (replace .cpp with .o)
OBJ = $(SRC:.cpp=.o)

# Default target: build the executable
all: $(OUT)

# Rule to link object files into the final executable
$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(OBJ) $(OPENSSL_LDFLAGS)

# Rule to compile each .cpp file into .o object file
%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -f $(OBJ) $(OUT)

# Rule for re-building everything (including header files)
rebuild: clean all
