# C++ Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++20 -g -O3
CPP_OBJECTS = tee.o
CPP_PROGRAMS = tee

# Rust Variables
RUSTC = rustc
RUST_PROGRAM = tee_rust
RUST_SRC = tee.rs

# a good question to ask is what are CXX and CXXFLAGS?

# Targets and Dependencies - implement each of these below

all: $(CPP_PROGRAMS) $(RUST_PROGRAM)

# C++ Build Targets
tee: tee.o
	$(CXX) $(CXXFLAGS) -o tee tee.o 

tee.o: tee.cpp
	$(CXX) $(CXXFLAGS) -c tee.cpp

# Rust Build Targets
tee_rust: $(RUST_SRC)
	$(RUSTC) $(RUSTFLAGS) -o tee_rust $(RUST_SRC)

clean: 
	rm -f $(CPP_PROGRAMS) $(CPP_OBJECTS) $(RUST_PROGRAM)

.PHONY: all clean
