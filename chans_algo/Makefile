CXX = g++
INC_DIR = ./include
SRC_DIR = ./src
BUILD_DIR = ./build
CXXFLAGS = -c -Wall -O3 -I$(INC_DIR)
INCLUDES = $(INC_DIR)/*.hpp
BUILD = $(BUILD_DIR)/*.o

chan: $(BUILD)
	$(CXX) $(BUILD) -o main_chan

build/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) $< -o $@