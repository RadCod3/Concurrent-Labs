#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Node struct for the linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Linked list structure
typedef struct {
    Node* head;
} LinkedList;

// Function prototypes
void init_list(LinkedList* list);          // Initialize the list
void free_list(LinkedList* list);          // Free the list memory
int member(LinkedList* list, int value);   // Check if a value is in the list
void insert(LinkedList* list, int value);  // Insert a value into the list
void delete(LinkedList* list, int value);  // Delete a value from the list

#endif