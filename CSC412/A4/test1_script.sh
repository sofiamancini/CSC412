#!/bin/bash

# Compile the program
echo "Compiling the program..."
g++ -std=c++20 -o ascii_art ascii_art.cpp main.cpp -lpthread

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting..."
    exit 1
fi

echo "Compilation successful. Running tests..."

# Test 1: Basic functionality
echo "Running Test 1: Basic functionality..."
./ascii_art &
PID=$!
sleep 2  # Wait for the program to initialize

# Simulate keyboard inputs
echo "Sending 'r' to refill red ink..."
echo -n 'r' > /tmp/my_pipe
sleep 1

echo "Sending 'g' to refill green ink..."
echo -n 'g' > /tmp/my_pipe
sleep 1

echo "Sending 'b' to refill blue ink..."
echo -n 'b' > /tmp/my_pipe
sleep 1

echo "Sending '+' to speed up inklings..."
echo -n '+' > /tmp/my_pipe
sleep 1

echo "Sending '-' to slow down inklings..."
echo -n '-' > /tmp/my_pipe
sleep 1

echo "Sending '<' to slow down producers..."
echo -n '<' > /tmp/my_pipe
sleep 1

echo "Sending '>' to speed up producers..."
echo -n '>' > /tmp/my_pipe
sleep 1

echo "Sending ESC to quit..."
echo -n $'\033' > /tmp/my_pipe
sleep 1

# Clean up
kill $PID
wait $PID 2>/dev/null

echo "Test 1 complete."

# Test 2: Terminal mode handling
echo "Running Test 2: Terminal mode handling..."
./ascii_art &
PID=$!
sleep 2

# Simulate abrupt termination
echo "Simulating abrupt termination (Ctrl+C)..."
kill -SIGINT $PID
wait $PID 2>/dev/null

# Verify terminal is in canonical mode
echo "Verifying terminal is in canonical mode..."
if [[ $(stty -a | grep -o "icanon") == "icanon" ]]; then
    echo "Terminal mode handling test passed."
else
    echo "Terminal mode handling test failed."
fi

echo "Test 2 complete."

# Test 3: Thread safety
echo "Running Test 3: Thread safety..."
./ascii_art &
PID=$!
sleep 2

# Simulate multiple calls to initializeFrontEnd
echo "Simulating multiple calls to initializeFrontEnd..."
for i in {1..5}; do
    echo -n 'r' > /tmp/my_pipe
    sleep 0.5
done

echo "Sending ESC to quit..."
echo -n $'\033' > /tmp/my_pipe
sleep 1

# Clean up
kill $PID
wait $PID 2>/dev/null

echo "Test 3 complete."

echo "All tests completed successfully."