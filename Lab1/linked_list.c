#include "linked_list.h"

#include <stdio.h>   // For printf
#include <stdlib.h>  // For malloc, free

// Initialize the list
void init_list(LinkedList* list) { list->head = NULL; }

// Free the list memory
void free_list(LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

// Check if a value is in the list
int member(LinkedList* list, int value) {
    Node* current = list->head;
    while (current != NULL) {
        if (current->data == value) {
            return 1;  // Value found
        }
        current = current->next;
    }
    return 0;  // Value not found
}

// Insert a value into the list (if not already present)
void insert(LinkedList* list, int value) {
    if (!member(list, value)) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = value;
        newNode->next = list->head;
        list->head = newNode;
    }
}

// Delete a value from the list
void delete(LinkedList* list, int value) {
    if (list->head == NULL) return;  // List is empty

    // Special case: If the value is in the head node
    if (list->head->data == value) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
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
}