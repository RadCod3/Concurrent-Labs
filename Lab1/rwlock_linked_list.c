#include "rwlock_linked_list.h"

#include <stdlib.h>

// Initialize the read-write lock-based linked list
void init_rwlock_list(RWLockLinkedList *list)
{
    list->head = NULL;
    pthread_rwlock_init(&list->rwlock, NULL); // Initialize the read-write lock
}

// Free the memory of the read-write lock-based linked list
void free_rwlock_list(RWLockLinkedList *list)
{
    pthread_rwlock_wrlock(&list->rwlock); // Lock for writing (deleting)
    Node *current = list->head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    pthread_rwlock_unlock(&list->rwlock);  // Unlock
    pthread_rwlock_destroy(&list->rwlock); // Destroy the read-write lock
}

// Check if a value is in the list (read operation)
int rwlock_member(RWLockLinkedList *list, int value)
{
    pthread_rwlock_rdlock(&list->rwlock); // Lock for reading
    Node *current = list->head;
    while (current != NULL)
    {
        if (current->data == value)
        {
            pthread_rwlock_unlock(&list->rwlock); // Unlock after reading
            return 1;                             // Value found
        }
        current = current->next;
    }
    pthread_rwlock_unlock(&list->rwlock); // Unlock after reading
    return 0;                             // Value not found
}

// Insert a value into the list (write operation)
void rwlock_insert(RWLockLinkedList *list, int value)
{
    pthread_rwlock_wrlock(&list->rwlock); // Lock for writing at the start

    // Check for the existence of the value under the write lock
    Node *current = list->head;
    while (current != NULL)
    {
        if (current->data == value)
        {
            pthread_rwlock_unlock(&list->rwlock); // Unlock and return if found
            return;
        }
        current = current->next;
    }

    // Proceed to insert since value is not found
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        pthread_rwlock_unlock(&list->rwlock); // Ensure to unlock before return on error
        return;                               // Handle memory allocation failure appropriately
    }
    newNode->data = value;
    newNode->next = list->head;
    list->head = newNode;

    pthread_rwlock_unlock(&list->rwlock); // Unlock after insertion
}

// Delete a value from the list (write operation)
void rwlock_delete(RWLockLinkedList *list, int value)
{
    pthread_rwlock_wrlock(&list->rwlock); // Lock for writing
    if (list->head == NULL)
    {
        pthread_rwlock_unlock(
            &list->rwlock); // Unlock and return if list is empty
        return;
    }

    if (list->head->data == value)
    {
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp);
        pthread_rwlock_unlock(&list->rwlock); // Unlock after deleting
        return;
    }

    Node *current = list->head;
    while (current->next != NULL && current->next->data != value)
    {
        current = current->next;
    }

    if (current->next != NULL)
    {
        Node *temp = current->next;
        current->next = current->next->next;
        free(temp);
    }

    pthread_rwlock_unlock(&list->rwlock); // Unlock after writing
}