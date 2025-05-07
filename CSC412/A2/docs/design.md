Intro

The 1982 UNIX demo has had far reaching influence on the field of software development. This assignment is made to mirror that demo and demonstrate several of the key functionalities Brian Kernighan implemented. 
The key purpose of this assignment is to demonstrate the power of functionalities such as piping and the breakdown of complex problems into simplier components. 
There are several parts of this design that will need to be addressed.
- Learning to use I/O Commands in C and Bash.
- Working with files, paths, and directories.
- Piping between C programs and Bash scripts.
- Writing new components to a C library.
- Begin to write more performant C and Bash.

Planning:

- I will be following the suggestions laid out in the assignment description and begin by implementing the C programs, in the order provided. 
- I expect the first 3-4 programs will be very simple to solve as they are familiar algorithms that I have implemented many times.
- Before I begin the programs, I will spend the time to research the most efficient algorthims for things such as sorting and searching in order to optimize my code before I begin writing it.
- I will also need to test each program along the way to first ensure they work as expected and then to see if they are able to operate in tandem.
- My experience with Bash is extremely limited so I think it will be worthwhile for me to spend time understanding some of the basic macros available to me before attempting to write any code.

C Programs:

- sentence
    - This should operate exactly as my readfile program did from Lab 2 so I will reuse that code.
- makewords
    - This should operate like the linebreaker code from Lab 2 so I will reuse that code as well.
- lowercase
    - My initial thoughts are to iterate through each word, using an if statement to convert any uppercase letters based on their ASCII values.
    - I will need to test this with punctuation, special characters, etc. to make sure it doesn't cause any unexpected errors.
- sort
    - I cannot remember the relative operational efficient of each sorting algorithm, so I will need to spend time to research that.
    - I will likely use a void functions that takes an array as an argument and compares the value of the first character in each item in the array.
- unique
    - Since at this point my input will be alphabetized, any duplicate words would be next to each other.
    - I will likely use a void function that takes in an array as an argument and compares each item with it's neighbor, removing duplicates as it iterates.
- mismatch
    - Since unix_dict.text is already alphabetized, I will use a search algorithm that compares the first word to the dictionary file until that word is found or the next word is (alphabetically) greater than the searched word, (meaning the word doesn'exist). Then start from that point and the the beginning, to search for the next word in the list.

Bash Scripts

The logic for the Bash scripts I expect to be the same, I will need to take the time to understand how to write more complex functions in Bash.

FIO C Library

- createFile
    - Based on the existing openFile function, I believe the c macro 'fopen' has a mode that specifically creates a blank file. I will use the same layout as the openFile to write this function.
- writeToFile
    - Within each of our program files we have used fprintf to write data into stdout, I will use this same macro to write data to a file. 
