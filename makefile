FILES = main.o Gene.o

CXXFLAGS = -std=c++14 -O3 -Wall -g -fsanitize=undefined -D_GLIBCXX_DEBUG
CXX = g++

.SUFFIXES: .cpp .o

run: $(FILES)
	$(CXX) $(CXXFLAGS) $(FILES) -o main
	./main

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $^ -o $@ 
