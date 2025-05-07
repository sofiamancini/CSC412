Design Document for A1: Reverse Test

Sofia Mancini
Due: 4th February, 2025
Course: CSC 412 Operating Systems and Networks

Overview:

The objective of this program is to implement five distinctive methods for reversing a character array and five different methods for reversing objects in an array. This program will need to correctly handle edge cases, bad input, and special cases.


Approach:

Inital Research: 
Before beginning this assignment, I reviewed the use of pointers in C, with special attention to pointer arthmetic, double points, and methods of memory manipulation. This included:
- Googleing double pointers.
- Reviewing my previous work.

Prototyping:

I began my implementation process by designing and testing the two swap functions. The logic behind these functions is something I have used several times in the past so these were fairly straightforward. I added printf lines to my function to test them at this stage.

Plans:

I plan to use five distinct methods for array reversal, adapting the algorithms to work for either multiple elements in an array or character arrays ('strings').
1. Basic swap - iteratively swapping elements from either end of the array.
2. Recursive swap - reversing the array recursively
3. Pointer arthimetic - using pointer math to reverse elements rather than indexing.
4. Temporary Array - creating an empty temporary array to store elements during the reversal.
5. XOR Method - Using an exclusive OR function to swap elements without the use of a temporary variable.

Each method will need to pass all cases, including edge and special characters.

Challenges:

Whitespace & Null Terminators

My initial tests ran into issues specifically with the leading & trailing spaces tests. My current suspicion is that the null terminator is causing issues where the whitespace isn't correctly being accounted for. I will need to re-read the null character section in the assignment and do some testing to determine exactly where this issue is happening.

Handling Special Characters:

My current implementation is returning garbage values for the two special cases. I believe this is because I am currently using memory locations for my swap, and non-ASCII values are not being handled correctly during this swap. I will implement a helper function to check for special characters and swap them out without the use of memory locations.

Function Details:

Swap Function:
- Using double pointers, swap char* 'string' in an array, rather than individual characters.

I used this article and examples to better understand how double pointers in c work:
https://www.geeksforgeeks.org/c-pointer-to-pointer-double-pointer/


Swap_chr Function:
- a and b in this function are both pointers to specific characters within the array. The swap_chr function works by passing the memory address of these characters and directly modifying the string. By assigning char temp = *a, the a is deferenced and the actual character is being passed along.

Edge Cases:
- In order to handle the tests with incorrect data, I will use a helper function (handle_edge_cases(char* str, int size)) that will use the size of the input to determine which cases need correcting, and replace the 's' with 'S'.

Special Characters:
- To identify the special cases that contain non-ASCII characters I will use a helper function (check_chars(char* str)) that will iterate through the character array and check if each element is a number between 0-127, since those are the ascii character values, and implement a different strategy for those special cases.