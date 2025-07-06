# Define the libraries to link against #
LDFLAGS = -lCppUTest -lCppUTestExt
########################################

# Define the C,C++ & its flags to compile agaisnt #
CC = gcc
CFLAGS = -g -std=c17 -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wsign-conversion -Wconversion

# Define the c++ and its flags to compile agaisnt #
CXX = g++
CXXFlags = -g -std=c++17 -Wall -Wextra -Wpedantic -Wmissing-declarations
###################################################

# Final Target name which is executable and other code folders
Target = ./builds/tests
SOURCE = ./src
BUILDS = ./builds
TESTS = ./tests
###################################################

# Fetch all C filenames and equivlent object names
CSRCS = $(wildcard $(SOURCE)/*.c)
COBJS = $(patsubst $(SOURCE)/%.c, $(BUILDS)/%.o, $(CSRCS))

# Fetch all CPP filenames and equivlent object names
CXXSRCS = $(wildcard $(TESTS)/*.cpp)
CXXOBJS = $(patsubst $(TESTS)/%.cpp, $(BUILDS)/%.o, $(CXXSRCS))

# Build final Target
$(Target): $(COBJS) $(CXXOBJS)
	$(CXX) $(CXXFlags) $^ -o $@ $(LDFLAGS)

# Build object file for every matching C file
$(BUILDS)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build object file for every matching C++ file
$(BUILDS)/%.o: $(TESTS)/%.cpp
	$(CXX) $(CXXFlags) -c $< -o $@

# Target to run
run: $(Target)
	$(Target)

runVerbose: $(Target)
	$(Target) -v

checkLeaks: $(Target)
	valgrind --leak-check=full --show-leak-kinds=all $(Target)

# Usual cleaners
clean:
	rm -f ./builds/*

.PHONY: clean

#print:
#	$(info $(COBJS) $(CXXOBJS))
# 	$(info $(CXXOBJS) - $(COBJS))