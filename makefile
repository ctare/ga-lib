SRC_DIR = .
OBJ_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRC_FILES))

CXXFLAGS = -std=c++14 -O3 -Wall -g -fsanitize=undefined -D_GLIBCXX_DEBUG
CXX = g++

.SUFFIXES: .cpp .o

run: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o main
	./main

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MP $< -o $@

-include $(DEP_FILES) 
