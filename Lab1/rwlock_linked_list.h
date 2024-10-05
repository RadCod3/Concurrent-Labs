#ifndef RWLOCK_LINKED_LIST_H
#define RWLOCK_LINKED_LIST_H

#include <pthread.h>

#include "node.h"

// Read-write lock-based linked list structure
typedef struct {
    Node* head;
    pthread_rwlock_t rwlock;  // Read-write lock for the entire list
} RWLockLinkedList;

// Function prototypes for the read-write lock-based linked list
void init_rwlock_list(RWLockLinkedList* list);
void free_rwlock_list(RWLockLinkedList* list);
int rwlock_member(RWLockLinkedList* list, int value);
void rwlock_insert(RWLockLinkedList* list, int value);
void rwlock_delete(RWLockLinkedList* list, int value);

#endif