
# Reflection for A3: Shell

Sofia Mancini
Due: 26th February, 2025
Course: CSC 412 Operating Systems and Networks

## Overview:

My planning for this assignment helped tremendously for the creation of the GNU Utilities. The most challenging part of this assignment was the shell file as it involved solutions I had never used before as well as several workarounds that took a not inconsiderable amount of time to work out.

## Design Doc Updates:

The C++ programs all were fairly easy to write and implement. The most complicated part of this process was ensuring I had the correct errors set for all edge cases. My initial program designs also did not account for multiple files as input, so I had to add in additional loops to account for this as well as handling bad input cases.

For lab 3 I had used both C++ and Rust so I already had a framework for the Makefile that included multiple languages, so including C, C++, and Rust into the Makefile for this assignment was straightforward.

While I had mentioned in my Design Doc that I believed the Shell file would be the most difficult part, I underestimated how much time it would take for me to understand and implement the new features in this program.


## Research:

The Shell program required the most time and effort in research both prior to writing the actual code and for handling warnings and errors while implementing my designs. I began with researching the system calls suggested in the assignment including fork, execv, dup2, waitpid, etc. Another resource I found really helpful was: https://brennan.io/2015/01/16/write-a-shell-in-c/ this contains lots of examples of implementing shells as well as well-described logic and functionalities. Using the stencil and these examples, I was able to build a majority of the Shell framework.

The first set of issues I encountered were entirely my own fault, I would compile and run my code perfectly fine but would fail Gradescope tests. This was because I was running the programs in my own environment and not within the container. I was trying to be lazy and corrected that mistake quickly. Running my code again inside the container would result in new warnings and errors I had not seen previously. 

My first issue was warnings regarding memory handling and my use of strtok_r. After a lot of frustration I realized it was due to the -std=c11 flag in the Makefile. I eventually found this chat: https://stackoverflow.com/questions/53037824/in-contrast-to-c-code-why-c-code-doesnt-need-define-posix-c-source-200809 that suggested the #define _POSIX_C_SOURCE 200809L addition as well as an custom function to handle string copying. I also created another function that handles memory allocation/deallocation. This cleaned up my other functions and ensured I was handling all memory issues in one location. 

When converting my C++ programs to Rust there were a few optimizations I found that helped me streamline my code. The first was traits: https://doc.rust-lang.org/std/default/trait.Default.html the #[derive(Default)] flag was really useful in the wc program to quickly initialize variables. I also found out about filter maps: https://stackoverflow.com/questions/62339080/understanding-filter-map-in-rust which are a more convinent way to implement multiple functionalities in the same process.


## Time Frame:

The C++ functions took about 1 hour for the initial design and another 30 minutes for formatting and edge case handling.

The Makefile was almost identical to my previous one and took under 30 minutes to include the additional programs.

The Rust programs required I spend a bit of time looking into the documentation, so altogether those programs took me 2-3 hours to complete.

The Shell file required a lot of effort, research, and reworking. I completed it over the weekend, dedicating roughly 3 hours each day, as well as about 2 hours prior to writing for research and design. Bug handling took another 3-4 hours to fix.

## UNIT test:

Purpose: The purpose of this test will be to test the functionality of C++ and Rust files as well as the ability for Makefile and 412shell to compile, run, and execute various tests.

Input: 
- Test case 1: test.txt
- Test case 2: large_file.txt
- Test case 3: stdin > " Holy smokes, you just coded in C and used Bash, congratulations! How many new things did you do today? "
- Test case 4: test.txt large_file.txt bad_input.test

Expected Output:

Each C++ and Rust program should correctly parse the input or stdin or execute the functions of that program.
Makefile should compile all programs and produce object and executable files. It should also correctly remove these files with 'make clean'
412shell should open a shell program and correctly handle the implemented GNU utils.

Explanation: These inputs test my functions against standard inputs, incorrect inputs, and handles edge cases. Confirmation that these tests succeed show the correctness of my functions.