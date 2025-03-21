# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# OpenSSL paths (adjust these paths if necessary)
OPENSSL_CFLAGS = $(shell pkg-config --cflags openssl)
OPENSSL_LDFLAGS = $(shell pkg-config --libs openssl)

# Source files for main1 and main2
SRC1 = pibas.cpp encryption.cpp main_static_dataset.cpp
SRC2 = pibas.cpp encryption.cpp main_static_range.cpp

# Header files (optional, but included for clarity)
HDR = pibas.hpp encryption.hpp

# Output executables
OUT1 = main_static_dataset
OUT2 = main_static_range

# Object files (replace .cpp with .o)
OBJ1 = $(SRC1:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)

# Default target: build both executables
all: $(OUT1) $(OUT2)

# Rule to link object files into the final executable for main1
$(OUT1): $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $(OUT1) $(OBJ1) $(OPENSSL_LDFLAGS)

# Rule to link object files into the final executable for main2
$(OUT2): $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $(OUT2) $(OBJ2) $(OPENSSL_LDFLAGS)

# Rule to compile each .cpp file into .o object file
%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

# Clean target to remove object files and executables
clean:
	rm -f $(OBJ1) $(OBJ2) $(OUT1) $(OUT2)

# Rule for re-building everything (including header files)
rebuild: clean all

