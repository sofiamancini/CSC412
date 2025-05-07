#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int length_list(node_t* head_list) {
    int len = 0;
    if (!head_list) {
        return len;
    }
    node_t* current = head_list;
    while (current) {
        len++;
        current = current->next; 
    }
    return len;  // Removed the free(current)
}

void* get_first(node_t* head_list) { 
    if (!head_list) { 
        return NULL; 
    }
    return head_list->data; 
}

void* get_last(node_t* head_list) {
    if (!head_list) {
        return NULL;
    }
    
    node_t* curr = head_list;
    while (curr) {  
        if (!curr->next) {  
            return curr->data;
        }
        curr = curr->next;
    }
    
    return NULL;
}

int insert_first(node_t** head_list, void* to_add, size_t size) {
    if (!to_add || !head_list) return 0;

    node_t* new_element = malloc(sizeof(node_t));
    if (!new_element) return 0;
    
    void* new_data = malloc(size);
    if (!new_data) {
        free(new_element);
        return 0;
    }
    
    memcpy(new_data, to_add, size);
    new_element->data = new_data;
    new_element->next = *head_list;
    new_element->prev = NULL;

    if (*head_list) {
        (*head_list)->prev = new_element;
    }

    *head_list = new_element;
    return 1;  // Success
}

void insert_last(node_t** head_list, void* to_add, size_t size) {
    if (!to_add || !head_list) return;

    node_t* new_element = malloc(sizeof(node_t));
    if (!new_element) return;
    
    void* new_data = malloc(size);
    if (!new_data) {
        free(new_element);
        return;
    }
    
    memcpy(new_data, to_add, size);
    new_element->data = new_data;
    new_element->next = NULL;

    if (!(*head_list)) {
        new_element->prev = NULL;
        *head_list = new_element;
        return;
    }

    node_t* curr = *head_list;
    while (curr->next) {
        curr = curr->next;
    }

    curr->next = new_element;
    new_element->prev = curr;
}

void* get(node_t* head_list, int index) {
    if (!head_list || index < 0) return NULL;

    node_t* curr = head_list;
    int i = 0;

    while (curr) {
        if (i == index) {
            return curr->data;
        }
        curr = curr->next;
        i++;
    }

    return NULL;
}

int remove_element(node_t** head_list, void* to_remove, size_t size) {
    if (!head_list || !(*head_list) || !to_remove) return 0;

    node_t* curr = *head_list;
    while (curr) {
        if (!memcmp(curr->data, to_remove, size)) {
            if (curr->next) {
                curr->next->prev = curr->prev;
            }
            if (curr == *head_list) {
                *head_list = curr->next;
            } else {
                curr->prev->next = curr->next;
            }

            free(curr->data);
            free(curr);
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void reverse_helper(node_t** head_list) {
    if (!head_list || !(*head_list)) return;

    node_t* curr = *head_list;
    node_t* placeholder = NULL;

    while (curr) {
        node_t* next_node = curr->next;
        curr->next = placeholder;
        curr->prev = next_node;
        placeholder = curr;
        curr = next_node;
    }
    *head_list = placeholder;
}

void reverse(node_t** head_list) {
    if (head_list) {
        reverse_helper(head_list);
    }
}

void* remove_first(node_t** head_list) {
    if (!head_list || !(*head_list)) return NULL;

    node_t* curr = *head_list;
    void* data = curr->data;  
    *head_list = curr->next;
    
    if (*head_list) {
        (*head_list)->prev = NULL;
    }

    free(curr);  
    return data;  
}

void* remove_last(node_t** head_list) {
    if (!head_list || !(*head_list)) return NULL;

    node_t* curr = *head_list;
    while (curr->next) {
        curr = curr->next;
    }

    void* data = curr->data;
    if (curr->prev) {
        curr->prev->next = NULL;
    } else {
        *head_list = NULL;
    }

    free(curr);
    return data;
}