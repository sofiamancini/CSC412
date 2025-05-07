#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1000
#define BUFFER_SIZE 4192

void merge(char **arr, char **aux, int left, int mid, int right) {
    memcpy(aux + left, arr + left, (right - left + 1) * sizeof(char *)); // More efficient copy

    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        arr[k++] = (strcmp(aux[i], aux[j]) <= 0) ? aux[i++] : aux[j++];
    }
    while (i <= mid) arr[k++] = aux[i++];
}

// Merge Sort function
void mergeSort(char **arr, char **aux, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, aux, left, mid);
        mergeSort(arr, aux, mid + 1, right);
        merge(arr, aux, left, mid, right);
    }
}

int main() {
    char buffer[BUFFER_SIZE];
    int capacity = INITIAL_CAPACITY, word_count = 0;
    char **words = malloc(capacity * sizeof(char *));
    char **aux = malloc(capacity * sizeof(char *)); // Auxiliary array for merge sort

    if (!words || !aux) return 1;

    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        char *token = strtok(buffer, " \t\n");
        while (token) {
            if (word_count >= capacity) {
                capacity *= 2;
                words = realloc(words, capacity * sizeof(char *));
                aux = realloc(aux, capacity * sizeof(char *)); // Ensure aux scales as well
                if (!words || !aux) return 1;
            }
            words[word_count++] = strndup(token, BUFFER_SIZE);
            token = strtok(NULL, " \t\n");
        }
    }

    mergeSort(words, aux, 0, word_count - 1);

    for (int i = 0; i < word_count; i++) {
        puts(words[i]);
        free(words[i]);
    }

    free(words);
    free(aux);
    return 0;
}