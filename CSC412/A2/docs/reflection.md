Reflection for A2: UNIX Demo

Sofia Mancini
Due: 18th February, 2025
Course: CSC 412 Operating Systems and Networks

Overview:

Looking back through my Design Doc, I feel like my initial plans helped me to work through this assignment. By doing research ahead of time and re-familiarizing myself with common sorting algorthims helped tremendously when it came to the implementation. 

Design Doc Updates:

The program that changed the most dramatically from my initial plan was mismatch. While my original idea was solid, I had forgot to consider that I was using pointers. My first attempt was either not correctly identifying words or missing exisiting words because while the dictionary is organized alphabetically, the pointers referencing those values may not be sorted the same. In order to avoid this issue I ran an additional sorting algorithm on the dictionary entries so that they were sorted in relation to the pointers. 
I also didn't mention the BUFFER_SIZE in my Design Doc but I found that changing the value of that and my CAPACITY variable changed my runtime dramatically. Since the dictionary size is known I was able to optimize that value.  

Research:

Other than revisiting the various algorithms, that most time I spent researching was on solving memory and runtime issues. I used several methods for storage and accessing values and used methods like the macro memcpy() for reducing run time and allocating memory to an auxilary array outside of the function call to reduce usage on the stack.

Time Frame:

The new fio functions, sentence, makewords, and lowercase c files took around 30 minutes to design, test, and build.

The first design of the sort.c program took me about 1-2 hours to implement. Much of that time was spent researching different memory allocation techniques.

unique.c took about 15 minutes to complete since the research I conducted prior helped to design this program.

The mismatch.c program took about 1 hour to complete as I ran into issues where my original plan failed due to pointer values. 

The Bash files took about 2 hours of prior research, I also utilized my lecture notes from CSC 411 since we had just discussed the tr methods.
Finishing these scripts took an additional hour.

Resolving runtime and memory issues all together took roughly 4 hours to solve.

UNIT test:

Purpose: The purpose of this test will be to test the ability for the .c programs and Bash scripts to process large files efficiently. 

Input: 
I downloaded a book from AO3 that has a word count over 370k.

Expected Output:
A large text file containing all words that do not exist in the unix_dict.text file as well as the same information printed to the standard output.

Explanation: This file should be large enough to be used as a comparison to the large input file from Gradescope.