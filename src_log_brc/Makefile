CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Executables for each main function
TARGET1 = main_static_dataset
TARGET2 = main_static_range

# Source files for each target
SOURCES1 = main_static_dataset.cpp log_brc.cpp
SOURCES2 = main_static_range.cpp log_brc.cpp

# Headers
HEADERS = log_brc.hpp

# Default target: build both executables
all: $(TARGET1) $(TARGET2)

# Compilation rules
$(TARGET1): $(SOURCES1) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES1) -o $(TARGET1)

$(TARGET2): $(SOURCES2) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES2) -o $(TARGET2)



# Clean rule
clean:
	rm -f $(TARGET1) $(TARGET2)
