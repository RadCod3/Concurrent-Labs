#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"  // Include the shared Node structure

// Linked list structure
typedef struct {
    Node* head;
} LinkedList;

// Function prototypes
void init_list(LinkedList* list);
void free_list(LinkedList* list);
int member(LinkedList* list, int value);
void insert(LinkedList* list, int value);
void delete(LinkedList* list, int value);

#endif