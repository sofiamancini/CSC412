#! /usr/bin/env bash

### call the build script to compile all c code
./build.sh

### output to a text file using both programs
./readfile | ./linebreaker > output.text

### output to terminal using both programs
output=$(./readfile | ./linebreaker)
echo "$output"
