# C++ Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20 -g -O3
CPP_OBJECTS = tee.o head.o tail.o wc.o pwd.o
CPP_PROGRAMS = tee head tail wc pwd

# C Variables
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -O3
C_OBJECTS = 412shell.o
C_PROGRAMS = 412shell

# Rust Variables
RUSTC = rustc
RUSTFLAGS = -C opt-level=3
RUST_PROGRAM = tee_rust pwd_rust wc_rust head_rust tail_rust
RUST_SRC = tee.rs pwd.rs wc.rs head.rs tail.rs
# a good question to ask is what are CXX and CXXFLAGS?

# Targets and Dependencies - implement each of these below

all: $(CPP_PROGRAMS) $(C_PROGRAMS) $(RUST_PROGRAM)

# C++ Build Targets
tee: tee.o
	$(CXX) $(CXXFLAGS) -o tee tee.o 

head: head.o
	$(CXX) $(CXXFLAGS) -o head head.o

tail: tail.o
	$(CXX) $(CXXFLAGS) -o tail tail.o

wc: wc.o
	$(CXX) $(CXXFLAGS) -o wc wc.o

pwd: pwd.o
	$(CXX) $(CXXFLAGS) -o pwd pwd.o

tee.o: tee.cpp
	$(CXX) $(CXXFLAGS) -c tee.cpp

head.o: head.cpp
	$(CXX) $(CXXFLAGS) -c head.cpp

tail.o: tail.cpp
	$(CXX) $(CXXFLAGS) -c tail.cpp

wc.o: wc.cpp
	$(CXX) $(CXXFLAGS) -c wc.cpp

pwd.o: pwd.cpp
	$(CXX) $(CXXFLAGS) -c pwd.cpp

# C Build Targets
412shell: 412shell.o
	$(CC) $(CFLAGS) -o 412shell 412shell.o

412shell.o: 412shell.c
	$(CC) $(CFLAGS) -c 412shell.c

# Rust Build Targets
tee_rust: $(RUST_SRC)
	$(RUSTC) $(RUSTFLAGS) -o tee_rust tee.rs

pwd_rust: $(RUST_SRC)
	$(RUSTC) $(RUSTFLAGS) -o pwd_rust pwd.rs

wc_rust: $(RUST_SRC)
	$(RUSTC) $(RUSTFLAGS) -o wc_rust wc.rs

head_rust: $(RUST_SRC)
	$(RUSTC) $(RUSTFLAGS) -o head_rust head.rs

tail_rust: $(RUST_SRC)
	$(RUSTC) $(RUSTFLAGS) -o tail_rust tail.rs

# Clean Target

clean: 
	rm -f $(CPP_PROGRAMS) $(CPP_OBJECTS) $(C_PROGRAMS) $(C_OBJECTS) $(RUST_PROGRAM)

.PHONY: all clean