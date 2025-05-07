#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
// Because I know how long the dictionary is I can set the size to fit
#define DICT_SIZE 3100

// Merge sort to use on the dictionary
void merge(char **arr, char **aux, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;

    memcpy(aux + left, arr + left, (right - left + 1) * sizeof(char *)); // More efficient copy

    while (i <= mid && j <= right) {
        arr[k++] = (strcmp(aux[i], aux[j]) <= 0) ? aux[i++] : aux[j++];
    }
    while (i <= mid) arr[k++] = aux[i++];
}

void mergeSort(char **arr, char **aux, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, aux, left, mid);
        mergeSort(arr, aux, mid + 1, right);
        merge(arr, aux, left, mid, right);
    }
}

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    static char buffer[BUFFER_SIZE];
    static char dict_storage[DICT_SIZE][BUFFER_SIZE];
    static char *dict[DICT_SIZE], *aux[DICT_SIZE];
    int dict_count = 0;

    FILE *file = fopen("unix_dict.text", "r");
    if (!file) return 1;

    while (dict_count < DICT_SIZE && fgets(dict_storage[dict_count], BUFFER_SIZE, file)) {
        char *line = dict_storage[dict_count];
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        dict[dict_count++] = line;
    }
    fclose(file);

    mergeSort(dict, aux, 0, dict_count - 1);

    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *key = buffer;
        char **found = bsearch(&key, dict, dict_count, sizeof(char *), compare);
        if (!found) puts(buffer);
    }
    return 0;
}