#! /usr/bin/env bash

# build the test program revtest
build() {
    gcc -Wall -Werror revtest.c reverse.c -o revtest
}

build 

./revtest "$@"
