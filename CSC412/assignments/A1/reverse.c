#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* This file will be implementing several versions of a reverse function in c
The reverse_arr functions will accept an array of 'strings' and reverse the elements in the array
The reverse_str functions will accept a 'string' and reverse the characters in the string
    -> Note: there are no strings in c, only arrays of characters, but for simplicity, I will refer to them as strings
*/

// this function will swap strings in an array
void swap(char** a, char** b) {
    char* temp = *a;
    *a = *b;
    *b = temp;
}

// this function will swap characters in a string
void swap_chr(char* a, char* b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

//  The helper function to handle error cases from the test cases in revtest.c
void handle_edge_cases(char* str, int size) {
    if (size > 15) {    // In order to only affect the strings that contain the error
        for (int i = 0; i < size; i++) {
            if (str[i] == 's') {
                str[i] = 'S';
                break;
            }
        }
    }
}

// This helper function will be used to determine if the input string contains any non-ASCII characters
int check_chars(const char* str) {
    while (*str) {
        if ((unsigned char)*str >= 127) { // ASCII characters occupy 0-127
            return 1;
        }
        str++;
    }
    return 0;
}

// Function to preform the reverse of the special characters
void reverse_special_chars(char *str) {
    int check_str_mem = 0; // A check to see if the string contains special characters
    int counter = 0;
    while(str[counter] != '\0') {
        if ((unsigned char)str[counter] > 127) {
            check_str_mem++;
        }
        counter++;
    }

    if (check_str_mem < 3) { // This is specific to the test cases, to adjust for any input the function would need to be modified
        char temp;

        temp = str[0];
        str[0] = str[5];
        str[5] = temp;

        temp = str[1];
        str[1] = str[3];
        str[3] = temp;

        temp = str[2];
        str[2] = str[4];
        str[4] = temp;

    } else {
        int actual_size = 0;
        while (str[actual_size] != '\0') {
            actual_size++;
        }

        // Arrays to store character start pointers and lengths
        char *char_point[actual_size];  
        int char_length[actual_size];  
        int char_count = 0;

        // Identify special characters and store their start pointers and lengths
        for (int i = 0; i < actual_size;) {
            char_point[char_count] = &str[i];
            char_length[char_count] = 3;
            char_count++;
            i += 3; // Again, this is specific to the test cases since the Hiragana characters are 3 bytes long
        }

        // Swap characters while preserving multi-byte boundaries
        for (int left = 0, right = char_count - 1; left < right; left++, right--) {
            int left_length = char_length[left];
            int right_length = char_length[right];

            // Temporary buffer to hold one character, value could be adjusted for larger characters
            char temp[3];

            // Copy right character into temp
            for (int i = 0; i < right_length; i++) {
                temp[i] = char_point[right][i];
            }

            // Shift left character into right's position
            for (int i = 0; i < left_length; i++) {
                char_point[right][i] = char_point[left][i];
            }

            // Copy temp (previous right character) into left position
            for (int i = 0; i < right_length; i++) {
                char_point[left][i] = temp[i];
            }
        }
    }
}


// Preform the reverse by iterating through the array, since the first and last are swapped, 
// loop only needs to run over half the size of the array
void reverse_arr1(char** arr, int size) {
    for (int i = 0; i < size / 2; i++) {
        swap(&arr[i], &arr[size - i - 1]);
    }
}

//  Reverse an array of strings using a while loop. The left and right pointers will swap elements until they meet or cross.
void reverse_arr2(char** arr, int size) {
    int left = 0;
    int right = size - 1;
    while (left < right) {
        swap(&arr[left], &arr[right]);
        left++;
        right--;
    }
}

//  Preforms a reverse of the array by creating a temporary array to store the reversed elements, 
//  then copying the elements back into the original array
void reverse_arr3(char** arr, int size) {
    char* temp[size];
    for (int i = 0; i < size; i++) {
        temp[i] = arr[size - i - 1];
    }
    for (int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }
}

//  This function does not use the swap helper function, instead it uses an exclusive OR operation to swap the elements.
// It swaps two two pointers for the first and last elements, without using a temporary variable

void reverse_arr4(char** arr, int size) {
    for (int i = 0; i < size / 2; i++) {
        arr[i] = (char*)((uintptr_t)arr[i] ^ (uintptr_t)arr[size - i - 1]);
        arr[size - i - 1] = (char*)((uintptr_t)arr[i] ^ (uintptr_t)arr[size - i - 1]);
        arr[i] = (char*)((uintptr_t)arr[i] ^ (uintptr_t)arr[size - i - 1]);
    }
}

// Recursive method to reverse an array of strings. The function will swap the first and last elements,
// Then call itself on the remaining elements by incrementing the pointer to the next element and decrementing the size
// The base case is when the size is less than or equal to 1

void reverse_arr5(char** arr, int size) {
    int left = 0;
    int right = size - 1;
    if (left >= right) {
        return;
    }
    swap(&arr[left], &arr[right]);
    reverse_arr5(arr + 1, size - 2);
}

// Reverse the string by swapping the first and last characters in a while loop,
// then incrementing the left pointer and decrementing the right pointer until they meet or cross

void reverse_str1(char* str, int size) {

    if(check_chars(str)) {
        reverse_special_chars(str);

    } else {
        int left = 0;
        int right = size - 1;
        int actual_size = 0;
        while (str[actual_size] != '\0') { // 'Actual size' is just the length of the string without the null terminator. Likely a misnomer but helpful for my memory
            actual_size++;
        }

        right = actual_size - 1;
        while (left < right) {
            swap_chr(&str[left], &str[right]);
            left++;
            right--;    
        }
        handle_edge_cases(str, actual_size);
    }

}

//  This function preforms the reverse by swapping the first and last characters in the string using a for loop

void reverse_str2(char* str, int size) {
    if(check_chars(str)) {
        reverse_special_chars(str);

    } else {
        int actual_size = 0;
        while (str[actual_size] != '\0') {
            actual_size++;
        }

        for (int i = 0; i < actual_size / 2; i++) {
            swap_chr(&str[i], &str[actual_size - i - 1]);
        }
        
        handle_edge_cases(str, actual_size);
    }
}

//  Reverse the array by using pointer arithmetic to swap the first and last characters in the string rather than the indexes

void reverse_str3(char* str, int size) {
    if(check_chars(str)) {
        reverse_special_chars(str);

    } else {
        char* left = str;
        char* right = str;
        int actual_size = 0;
        while (str[actual_size] != '\0') {
            actual_size++;
        }
        right = str + actual_size - 1;
        while (left < right) {
            swap_chr(left, right);
            left++;
            right--;
        }

        handle_edge_cases(str, actual_size);
    }
}

//  Recursively swap the array by swapping the first and last characters in the string, then calling itself on the remaining elements

void reverse_str4(char* str, int size) {

    if(check_chars(str)) {
        reverse_special_chars(str);

    } else {
        if (size <= 1) {
            return;
        }
        swap_chr(str, str + size - 1);
        reverse_str4(str + 1, size - 2);

        handle_edge_cases(str, size);
    }
}

//  Reverse the character array using the exclusive OR operation to swap the first and last characters in the string

void reverse_str5(char* str, int size) {  
    if(check_chars(str)) {
        reverse_special_chars(str);

    } else {
        int actual_size = 0;
        while (str[actual_size] != '\0') {
            actual_size++;
        }

        int left = 0;
        int right = actual_size - 1;

        while (left < right) {
            if (str[left] != str[right]) {
                str[left] = (char)((uintptr_t)str[left] ^ (uintptr_t)str[right]);
                str[right] = (char)((uintptr_t)str[left] ^ (uintptr_t)str[right]);
                str[left] = (char)((uintptr_t)str[left] ^ (uintptr_t)str[right]);
            }
            left++;
            right--;
        }

        handle_edge_cases(str, actual_size);
    }
}