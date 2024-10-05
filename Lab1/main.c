#include <stdio.h>
#include <stdlib.h>  // For rand() and srand()
#include <time.h>    // For time()

#include "linked_list.h"

int main() {
    // Seed the random number generator
    srand((unsigned int)time(0));

    // Create and initialize the linked list
    LinkedList list;
    init_list(&list);

    // Populate the list with n unique random values
    int n = 10;  // Number of initial elements to insert
    printf("Inserting %d unique random values into the list:\n", n);

    for (int i = 0; i < n; i++) {
        int value = rand() % 65536;  // Random value between 0 and 2^16 - 1
        if (!member(&list, value)) {
            insert(&list, value);
            printf("Inserted: %d\n", value);
        } else {
            i--;  // Retry insertion to ensure uniqueness
        }
    }

    // Perform m random operations (Member, Insert, Delete)
    int m = 20;           // Number of random operations
    float mMember = 0.6;  // 60% Member operations
    float mInsert = 0.2;  // 20% Insert operations
    float mDelete = 0.2;  // 20% Delete operations

    printf("\nPerforming %d random operations (Member, Insert, Delete):\n", m);

    for (int i = 0; i < m; i++) {
        float operationType = (float)rand() / RAND_MAX;
        int value = rand() % 65536;

        if (operationType < mMember) {
            // Member operation
            printf("Member(%d) -> %s\n", value,
                   member(&list, value) ? "Found" : "Not Found");
        } else if (operationType < mMember + mInsert) {
            // Insert operation
            if (!member(&list, value)) {
                insert(&list, value);
                printf("Insert(%d) -> Inserted\n", value);
            } else {
                printf("Insert(%d) -> Already Exists\n", value);
            }
        } else {
            // Delete operation
            if (member(&list, value)) {
                delete (&list, value);
                printf("Delete(%d) -> Deleted\n", value);
            } else {
                printf("Delete(%d) -> Not Found\n", value);
            }
        }
    }

    // Free the memory used by the linked list
    free_list(&list);

    return 0;
}