#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linked_list.h"
#include "mutex_linked_list.h"

// Parameters for testing
int n = 100;          // Number of initial elements to insert
int m = 1000;         // Total number of random operations
float mMember = 0.6;  // 60% Member operations
float mInsert = 0.2;  // 20% Insert operations
float mDelete = 0.2;  // 20% Delete operations

MutexLinkedList mutexList;

// Helper function to shuffle an array of characters (used to shuffle
// operations)
void shuffle(char* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Generate an exact sequence of mMember, mInsert, and mDelete operations
void generate_operations(char* operations) {
    int member_ops = m * mMember;
    int insert_ops = m * mInsert;
    int delete_ops = m * mDelete;

    // Fill the array with 'M' for Member, 'I' for Insert, and 'D' for Delete
    for (int i = 0; i < member_ops; i++) {
        operations[i] = 'M';
    }
    for (int i = member_ops; i < member_ops + insert_ops; i++) {
        operations[i] = 'I';
    }
    for (int i = member_ops + insert_ops; i < m; i++) {
        operations[i] = 'D';
    }

    // Shuffle the operations array to randomize the order
    shuffle(operations, m);
}

// Serial linked list operations
void test_serial_linked_list(LinkedList* list, char* operations) {
    for (int i = 0; i < m; i++) {
        int value = rand() % 65536;  // Random value between 0 and 2^16 - 1

        if (operations[i] == 'M') {
            // printf("Serial: Member(%d) -> %s\n", value,
            //        member(list, value) ? "Found" : "Not Found");
        } else if (operations[i] == 'I') {
            if (!member(list, value)) {
                insert(list, value);
                // printf("Serial: Insert(%d) -> Inserted\n", value);
            } else {
                // printf("Serial: Insert(%d) -> Already Exists\n", value);
            }
        } else if (operations[i] == 'D') {
            if (member(list, value)) {
                delete (list, value);
                // printf("Serial: Delete(%d) -> Deleted\n", value);
            } else {
                // printf("Serial: Delete(%d) -> Not Found\n", value);
            }
        }
    }
}

// Mutex-based linked list operations in each thread
void* thread_operations(void* arg) {
    char* operations = ((char**)arg)[0];
    int start = *((int*)((char**)arg)[1]);
    int end = *((int*)((char**)arg)[2]);

    for (int i = start; i < end; i++) {
        int value = rand() % 65536;  // Random value between 0 and 2^16 - 1

        if (operations[i] == 'M') {
            // printf("Thread: Member(%d) -> %s\n", value,
            //        mutex_member(&mutexList, value) ? "Found" : "Not Found");
        } else if (operations[i] == 'I') {
            if (!mutex_member(&mutexList, value)) {
                mutex_insert(&mutexList, value);
                // printf("Thread: Insert(%d) -> Inserted\n", value);
            } else {
                // printf("Thread: Insert(%d) -> Already Exists\n", value);
            }
        } else if (operations[i] == 'D') {
            if (mutex_member(&mutexList, value)) {
                mutex_delete(&mutexList, value);
                // printf("Thread: Delete(%d) -> Deleted\n", value);
            } else {
                // printf("Thread: Delete(%d) -> Not Found\n", value);
            }
        }
    }

    return NULL;
}

int main() {
    // Seed the random number generator
    srand((unsigned int)time(0));

    // Generate the exact operations sequence
    char operations[m];
    generate_operations(operations);

    // ---------- SERIAL TEST ----------
    LinkedList serialList;
    init_list(&serialList);

    // Populate the serial list
    for (int i = 0; i < n; i++) {
        int value = rand() % 65536;
        if (!member(&serialList, value)) {
            insert(&serialList, value);
        }
    }

    // Start timing for the serial test
    clock_t start_serial = clock();
    test_serial_linked_list(&serialList, operations);
    clock_t end_serial = clock();
    double time_serial = ((double)(end_serial - start_serial)) / CLOCKS_PER_SEC;
    printf("\nTime taken for serial operations: %f seconds\n", time_serial);

    // Free the serial linked list
    free_list(&serialList);

    // ---------- MUTEX-BASED TEST ----------
    init_mutex_list(&mutexList);

    // Populate the mutex-based list
    for (int i = 0; i < n; i++) {
        int value = rand() % 65536;
        if (!mutex_member(&mutexList, value)) {
            mutex_insert(&mutexList, value);
        }
    }

    // Start timing for the mutex-based test
    int num_threads = 4;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    int ops_per_thread = m / num_threads;

    clock_t start_mutex = clock();

    // Create threads and pass the portion of the operations array to each
    for (int i = 0; i < num_threads; i++) {
        int start = i * ops_per_thread;
        int end = (i == num_threads - 1) ? m : (i + 1) * ops_per_thread;

        void* args[3] = {operations, &start, &end};
        pthread_create(&threads[i], NULL, thread_operations, args);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_mutex = clock();
    double time_mutex = ((double)(end_mutex - start_mutex)) / CLOCKS_PER_SEC;
    printf("\nTime taken for mutex-based operations: %f seconds\n", time_mutex);

    // Free the mutex-based linked list
    free_mutex_list(&mutexList);

    return 0;
}