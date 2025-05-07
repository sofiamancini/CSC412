#! /usr/bin/env bash

### compile all c code in the current directory

# Set compiler and flags
CC=gcc
CFLAGS="-Wall -Wextra -O2"

# Define program names
PROGRAMS=("sentence" "makewords" "lowercase" "sort" "unique" "mismatch")

# Compile each program
for prog in "${PROGRAMS[@]}"; do
    $CC $CFLAGS "$prog.c" -o "$prog" || { echo "Compilation failed for $prog.c"; exit 1; }
done

# Define script names
# SCRIPTS=("sentence.sh" "makewords.sh" "lowercase.sh" "sort.sh" "unique.sh" "mismatch.sh")

# # Run each script
# for script in "${SCRIPTS[@]}"; do
#     bash "$script" || { echo "Execution failed for $script"; exit 1; }
# done

# Run the pipeline and output to stdout
# ./sentence.sh | ./makewords.sh | ./lowercase.sh | ./sort.sh | ./unique.sh | ./mismatch.sh

# ./sentence.sh | ./makewords.sh | ./lowercase.sh | ./sort.sh | ./unique.sh | ./mismatch.sh > unix_bash_output.text