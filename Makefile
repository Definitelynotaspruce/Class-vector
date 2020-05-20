CXX = g++
CXXFLAGS = -Wall -std=c++17 -pedantic
LD_FLAGS = -lgtest -lpthread

NAME = main

PATH_TO_SRC := src/
SRC_FILES = $(wildcard $(PATH_TO_SRC)*.cpp)

PATH_TO_TEST := testai/
TEST_FILES = $(wildcard $(PATH_TO_TEST)*.cpp)
TEST_ADDITIONAL = $(PATH_TO_SRC)Studentas.cpp

OBJECT_OUT = $(SRC_FILES:.cpp=.o)

default: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -o $(NAME)  $^ 

.PHONY: test
test: $(TEST_FILES)
	$(CXX) -o $@ $^ $(TEST_ADDITIONAL) $(CXXFLAGS) $(LD_FLAGS)

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
	rm -f test
	rm -R build