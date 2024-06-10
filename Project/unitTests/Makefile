# Compiler
CC = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Targets
MAIN_TARGET = main.out
TEST_TARGET = test_simulation.out

# Source Files
MAIN_SRCS = main.cpp simulation.cpp
TEST_SRCS = test_simulation.cpp simulation.cpp

# Object Files
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Default Target
all: $(MAIN_TARGET) $(TEST_TARGET)

# Main Target
$(MAIN_TARGET): $(MAIN_SRCS)
	$(CC) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_SRCS)

# Test Target
$(TEST_TARGET): $(TEST_SRCS)
	$(CC) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRCS)

# Clean
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) $(MAIN_OBJS) $(TEST_OBJS)

# Test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean test

