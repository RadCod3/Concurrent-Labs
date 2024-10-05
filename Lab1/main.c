#include <stdio.h>
#include <stdlib.h>  // For rand() and srand()
#include <time.h>    // For clock(), time()

#include "linked_list.h"

// Function to test the linked list operations
void test_linked_list(LinkedList* list, int n, int m, float mMember,
                      float mInsert, float mDelete) {
    // Populate the list with n unique random values
    // printf("Inserting %d unique random values into the list:\n", n);
    for (int i = 0; i < n; i++) {
        int value = rand() % 65536;  // Random value between 0 and 2^16 - 1
        if (!member(list, value)) {
            insert(list, value);
            // printf("Inserted: %d\n", value);
        } else {
            i--;  // Retry insertion to ensure uniqueness
        }
    }

    // Perform m random operations (Member, Insert, Delete)
    // printf("\nPerforming %d random operations (Member, Insert, Delete):\n",
    // m);
    for (int i = 0; i < m; i++) {
        float operationType = (float)rand() / RAND_MAX;
        int value = rand() % 65536;

        if (operationType < mMember) {
            // Member operation
            // printf("Member(%d) -> %s\n", value,
            //    member(list, value) ? "Found" : "Not Found");
        } else if (operationType < mMember + mInsert) {
            // Insert operation
            if (!member(list, value)) {
                insert(list, value);
                // printf("Insert(%d) -> Inserted\n", value);
            } else {
                // printf("Insert(%d) -> Already Exists\n", value);
            }
        } else {
            // Delete operation
            if (member(list, value)) {
                delete (list, value);
                // printf("Delete(%d) -> Deleted\n", value);
            } else {
                // printf("Delete(%d) -> Not Found\n", value);
            }
        }
    }
}

int main() {
    // Seed the random number generator
    srand((unsigned int)time(0));

    // Create and initialize the linked list
    LinkedList list;
    init_list(&list);

    // Parameters for testing
    int n = 1000;           // Number of initial elements to insert
    int m = 10000;          // Number of random operations
    float mMember = 0.99;   // 60% Member operations
    float mInsert = 0.005;  // 20% Insert operations
    float mDelete = 0.2;    // 20% Delete operations

    // Start the clock
    clock_t start = clock();

    // Test the linked list with the parameters
    test_linked_list(&list, n, m, mMember, mInsert, mDelete);

    // Stop the clock
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Output the time taken
    printf("\nTime taken for the operations: %f seconds\n", time_taken);

    // Free the memory used by the linked list
    free_list(&list);

    return 0;
}