CXX = g++
CXXFLAGS = -Wall -std=c++17 -pedantic

NAME = main

SRC_FILES = $(wildcard *.cpp)
OBJECT_OUT = $(SRC_FILES:.cpp=.o)

default: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -o $(NAME)  $^ 

.PHONY: build
build: $(SRC_FILES)
	mkdir -p build
	$(CXX) -c $^ $(CXXFLAGS)
	mv $(OBJECT_OUT) build

.PHONY: clean
clean:
	rm -f *.o $(NAME)

.PHONY: cleanall
cleanall:
	rm -f *.o $(NAME)
	rm -R build

