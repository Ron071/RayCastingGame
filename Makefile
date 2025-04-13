# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++23

# SFML directories
SFML_INCLUDE = /Users/ronaldgrinberg/Desktop/SFML-3.0.0/include
SFML_LIB = /Users/ronaldgrinberg/Desktop/SFML-3.0.0/lib

# Source files and target
SRC = main.cpp game.cpp maze.cpp player.cpp ray.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = game

# SFML libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -pthread

# Linker flags
LDFLAGS = -L$(SFML_LIB) -Wl,-rpath,$(SFML_LIB)

# Default target to build the game
all: $(TARGET)

# Build the target game
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Compile .cpp files to .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(SFML_INCLUDE) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild the game
rebuild: clean all
