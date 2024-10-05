#include "mutex_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// Initialize the list and mutex
void init_mutex_list(MutexLinkedList* list) {
    list->head = NULL;
    pthread_mutex_init(&list->mutex, NULL);  // Initialize the mutex
}

// Free the list memory and destroy the mutex
void free_mutex_list(MutexLinkedList* list) {
    pthread_mutex_lock(&list->mutex);  // Lock the mutex

    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;

    pthread_mutex_unlock(&list->mutex);   // Unlock the mutex
    pthread_mutex_destroy(&list->mutex);  // Destroy the mutex
}

// Check if a value is in the list (thread-safe)
int mutex_member(MutexLinkedList* list, int value) {
    pthread_mutex_lock(&list->mutex);  // Lock the mutex

    Node* current = list->head;
    while (current != NULL) {
        if (current->data == value) {
            pthread_mutex_unlock(&list->mutex);  // Unlock the mutex
            return 1;                            // Value found
        }
        current = current->next;
    }

    pthread_mutex_unlock(&list->mutex);  // Unlock the mutex
    return 0;                            // Value not found
}

// Insert a value into the list (thread-safe)
void mutex_insert(MutexLinkedList* list, int value) {
    if (!mutex_member(
            list, value)) {  // Only insert if value is not already in the list
        pthread_mutex_lock(&list->mutex);  // Lock the mutex

        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = value;
        newNode->next = list->head;
        list->head = newNode;
    }

    pthread_mutex_unlock(&list->mutex);  // Unlock the mutex
}

// Delete a value from the list (thread-safe)
void mutex_delete(MutexLinkedList* list, int value) {
    pthread_mutex_lock(&list->mutex);  // Lock the mutex

    if (list->head == NULL) {
        pthread_mutex_unlock(&list->mutex);  // Unlock the mutex
        return;                              // List is empty
    }

    // Special case: If the value is in the head node
    if (list->head->data == value) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);

        pthread_mutex_unlock(&list->mutex);  // Unlock the mutex
        return;
    }

    // Traverse the list to find the value
    Node* current = list->head;
    while (current->next != NULL && current->next->data != value) {
        current = current->next;
    }

    // If the value is found, remove the node
    if (current->next != NULL) {
        Node* temp = current->next;
        current->next = current->next->next;
        free(temp);
    }

    pthread_mutex_unlock(&list->mutex);  // Unlock the mutex
}