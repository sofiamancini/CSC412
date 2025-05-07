Reflection for A1: Reverse Test

Sofia Mancini
Due: 4th February, 2025
Course: CSC 412 Operating Systems and Networks

Overview:

While my initial plan for this assignment was solid, I encountered several issues that required more time and effort to resolve. Taking the time before my implementation to review pointers and references proved extremely useful in crafting efficient solutions.

Design Doc Updates:

My swap functions behaved as I expected them to and did not require any updates from my initial design. However, the first issue I ran into in my code was with handling the leading and trailing whitespace character tests. My code was not correctly identifying the null terminator and would result in the whitespace swaps not being handled correctly. When implementing my first solution I found that by declaring a new variable to count the number of characters in the array before the null terminator and using that variable to iterate through the loop solved this discrepancy. 
Later on, however, I chose to use a recursive function call for the reversal and using this same method results in an incorrect swap from test case 6. This implies that my solution is incomplete and there is a more effective method that could be used. Unfortunately, I do not currently have enough time to work through this issue. 

Research:

The most complicated issue I ran into was understanding and implementing a solution for the final two tests, 7 and 8. From what information I could find online I understand that special (non-ASCII) characters can have varying memory size. Knowing this I first attempted to create a helper function that would identify the byte-size of a given character and then use a typical swap method to swap the correct amount of space.

The issue this method ran into was when attempting to swap two characters of differing lengths. It would result in an extra memory block in one spot and the final result would have an additional nonsense character. 

To solve the swap method for the Hiragana characters I first learned they all occupied 3 bytes of memory, so using this I ran my loop incrementing by 3 each time to account for this memory allocation.

My solutions for the final two tests are hard-coded and are not applicable to varying inputs. With further time I could try to implement solutions that check for size differences in the swap characters and add in the correct memory spots to account for these varying legnths.

Time Frame:

The first five swap functions took me a total of about 1 hour to design, write, and test.

The second five functions took varying amounts of time and effort. To pass the first 4 tests using the first method took about 15 minutes total. Once I understood that the tests in 5 and 6 were designed to be incorrect finding a solution to work around that took about 10 minutes. To solve the issue with test 6 required some research and review on my part on null terminators as well as some adding some debugging lines into my code to work out exactly how to swap was failing. To come up with recalculating the size and implementing it took roughly 1 hour. 
The solution for the extra credit cases was created over several days as I tried several different methods before settling on the solution I ended up using.
Once I had one method that worked for all tests I began implementing the other varying methods, these went much quicker as I had already worked out the logic and research I needed. 

UNIT test:

Purpose: The purpose of this test will be to test the functionality of all ten reverse methods within this project.

Input: 
- Test case 1: ["I", "am", "tired."]
- Test case 2: "racecar"
- Test case 3: "laconic"
- Test case 4: "  Excogitate"
- Test case 5: "ねむいけどがんばる"

Expected Output:

- Test case 1: ["tired", "am", "I"]
- Test case 2: "racecar"
- Test case 3: "cinocal"
- Test case 4: " etatigocxE  "
- Test case 5: "るばんがどけいむね"

Explanation: These inputs test my functions against standard inputs, incorrect inputs, and handles edge cases. Confirmation that these tests succeed show the correctness of my functions.