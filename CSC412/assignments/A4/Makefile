# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20 -g -O3
INKLING_SOURCES = main.cpp ascii_art.cpp
LOG_SOURCES = logs.cpp
INKLING_TARGET = inklings
LOG_TARGET = logs

# Targets and Dependencies
all: $(INKLING_TARGET)

# Build the 'inklings' executable
$(INKLING_TARGET): $(INKLING_SOURCES)
	$(CXX) $(CXXFLAGS) $(INKLING_SOURCES) -o $(INKLING_TARGET) -lpthread

# Compile 'logs.cpp' into 'logs' executable
$(LOG_TARGET): $(LOG_SOURCES)
	$(CXX) $(CXXFLAGS) $(LOG_SOURCES) -o $(LOG_TARGET)


# Separate target for running 'inklings' program
run:
	./$(INKLING_TARGET)

# Separate target for running 'logs' program
run-logs: $(LOG_TARGET)
	./$(LOG_TARGET)

# Clean the build directory
clean:
	rm -f $(INKLING_TARGET) $(LOG_TARGET) *.o
	rm -rf inklings.dSYM logs.dSYM
	rm -rf logFolder
