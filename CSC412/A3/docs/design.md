# Design Document for A3: GNU Utilities

Sofia Mancini
Due: 25th February, 2025
Course: CSC 412 Operating Systems and Networks


## Intro

The purpose of this assignment is to recreate some of the most widely used GNU Utilities as well as creating our own Shell program. The GNU code can be written in C, C++, or Rust. The results of this assignment should be able to correctly work on very large text files.

### Objectives:
- Refresh and Improve your knowledge of C, C++, and/or Rust for systems programming
- Focus deeper on handling more complex and larger inputs to programs in userspace
- Learn how to create a super awesome Makefile
- Recreate popular GNU system utilities
- Expand your knowledge of interprocess communication (IPC)
- Learn Forking and Processes within your own Shell program written in C
- Learn how some common system calls work

## Planning:

- I will begin this assignment by first writing the GNU Utilities in C++. I will then create the Makefile needed to compile and run these programs. After testing the Makefile, I will create the Shell file. If I have time after successfully compiling and running these files, I will attempt to convert the C++ files into Rust and expanding the Makefile to compile Rust programs as well. 
- I expect the most difficult aspects of this program will be handling large examples and creating the Shell file. The previous assignment took some reworking from my original design in order to process large data and I expect similar results this time. I have never used fork or execv so I think understanding the requirements and then building the Shell program will be time consuming.
- I first need to take the time to research the GNU Utilities and test how the built-ins work before attempting to build them myself. Also, since it has been about a year since I've writeen code in C++ it will be worthwhile to revisit my past code examples to refresh on syntax and common differences between C and C++. Finally, I need to spend time to understand the function of the shell as well as the fork and execv functions.
- I will also need to test each program at each stage to ensure I am covering all edge cases and handling large inputs.

## C++ Programs:

- head - output the first 10 lines of files OR stdin if no files are provided.
int main(int argc, char *argv[]) {
    // if two arguments are given, file is the second argument
    file.open();
    // iterate through each line in file and print first ten lines to stdout

    // accept input from stdin if no file is given
    file.close();
    return 0;
}

- pwd - print name of current/working directory.
    - main function will return value of the active directory.

- tail - output the last 10 lines of files OR stdin if no files are provided.
int main(int argc, char *argv[]) {
    // if two arguments are given, file is the second argument
    file.open();
    // iterate through the input and print the last ten lines to stdout 

    // accept input from stdin if no file is given
    file.close();
    return 0;
}
- tee - read from standard input and write to standard output and files (taken as arguments). If not files are given, just write to stdout.
int main(int argc, char* argv[]) {
    // if two arguments are given, file is the second argument
    file.open();
    std::string line;
    // read from stdin and output to stdout and file (if given)
    // tee.cpp was written for lab3, i will use that function as a starting point.
    file.close();
    return 0;
}
- wc - print line, word, and byte counts for each file OR stdin if no files are provided.
    - accept any number of files and loop over each file
    - need to research how to find the line, word, and byte counts of a file.

## Makefile

A Makefile was written for lab3, I will use that as a basis for this assignment.
### Objectives:
- The makefile should be named Makefile.
- The object files (.o extension) and executables should be placed in the current directory
- The clean target should remove all object files and executables.
- For C++: Explicitly specify the compiler as g++.
- For C++: CXXFLAGS = -Wall -Wextra -pedantic and either
    -std=c++20 -g -O3, or
    -std=c++2a -g -O3
- For Rust: Explicitly specify the compiler as rustc.
    BTW for Rust your Makefile will need to compile your code with rustc and then move the executable to the correct location.

## Shell

This shell program will initiate and run the other programs that have been created. The shell should be able to execute any program (fail if it doesn't exist)

## Final Thoughts

After reviewing the full assignment details I think the key to being successful for me will be to do lots of testing within my own environment. Since there are many pieces to this assignment, waiting on the Gradescope compiler is too time consuming. I believe executing this code in Rust should be very manageable because my current working knowledge of Rust should be enough for the programs we're given.