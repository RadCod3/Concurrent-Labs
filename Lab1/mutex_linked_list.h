#ifndef MUTEX_LINKED_LIST_H
#define MUTEX_LINKED_LIST_H

#include <pthread.h>  // For pthread mutex

#include "node.h"  // Include the shared Node structure

// Mutex-based Linked list structure
typedef struct {
    Node* head;
    pthread_mutex_t mutex;  // Mutex to protect the linked list
} MutexLinkedList;

// Function prototypes for the mutex-based linked list
void init_mutex_list(MutexLinkedList* list);
void free_mutex_list(MutexLinkedList* list);
int mutex_member(MutexLinkedList* list, int value);
void mutex_insert(MutexLinkedList* list, int value);
void mutex_delete(MutexLinkedList* list, int value);

#endif