#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "linked_list.h"
#include "mutex_linked_list.h"
#include "rwlock_linked_list.h" // Include the read-write lock-based linked list

// Parameters for testing
int n = 1000;        // Number of initial elements to insert
int m = 10000;       // Total number of random operations
int runs = 200;      // Number of times to run the test
float mMember = 0.6; // 60% Member operations
float mInsert = 0.2; // 20% Insert operations
float mDelete = 0.2; // 20% Delete operations

MutexLinkedList mutexList;
RWLockLinkedList rwlockList;

// Helper function to shuffle an array of characters (used to shuffle
// operations)
void shuffle(char *array, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Generate an exact sequence of mMember, mInsert, and mDelete operations
void generate_operations(char *operations)
{
    int member_ops = m * mMember;
    int insert_ops = m * mInsert;
    int delete_ops = m * mDelete;

    // Fill the array with 'M' for Member, 'I' for Insert, and 'D' for Delete
    for (int i = 0; i < member_ops; i++)
    {
        operations[i] = 'M';
    }
    for (int i = member_ops; i < member_ops + insert_ops; i++)
    {
        operations[i] = 'I';
    }
    for (int i = member_ops + insert_ops; i < m; i++)
    {
        operations[i] = 'D';
    }

    // Shuffle the operations array to randomize the order
    shuffle(operations, m);
}

// Serial linked list operations
void test_serial_linked_list(LinkedList *list, char *operations)
{
    for (int i = 0; i < m; i++)
    {
        int value = rand() % 65536; // Random value between 0 and 2^16 - 1

        if (operations[i] == 'M')
        {
            member(list, value);
        }
        else if (operations[i] == 'I')
        {
            if (!member(list, value))
            {
                insert(list, value);
            }
        }
        else if (operations[i] == 'D')
        {
            if (member(list, value))
            {
                delete (list, value);
            }
        }
    }
}

// Mutex-based linked list operations in each thread
void *mutex_thread_operations(void *arg)
{
    char *operations = ((char **)arg)[0];
    int start = *((int *)((char **)arg)[1]);
    int end = *((int *)((char **)arg)[2]);

    for (int i = start; i < end; i++)
    {
        int value = rand() % 65536; // Random value between 0 and 2^16 - 1

        if (operations[i] == 'M')
        {
            mutex_member(&mutexList, value);
        }
        else if (operations[i] == 'I')
        {
            if (!mutex_member(&mutexList, value))
            {
                mutex_insert(&mutexList, value);
            }
        }
        else if (operations[i] == 'D')
        {
            if (mutex_member(&mutexList, value))
            {
                mutex_delete(&mutexList, value);
            }
        }
    }

    return NULL;
}

// Read-write lock-based linked list operations in each thread
void *rwlock_thread_operations(void *arg)
{
    char *operations = ((char **)arg)[0];
    int start = *((int *)((char **)arg)[1]);
    int end = *((int *)((char **)arg)[2]);

    for (int i = start; i < end; i++)
    {
        int value = rand() % 65536; // Random value between 0 and 2^16 - 1

        if (operations[i] == 'M')
        {
            rwlock_member(&rwlockList, value);
        }
        else if (operations[i] == 'I')
        {
            if (!rwlock_member(&rwlockList, value))
            {
                rwlock_insert(&rwlockList, value);
            }
        }
        else if (operations[i] == 'D')
        {
            if (rwlock_member(&rwlockList, value))
            {
                rwlock_delete(&rwlockList, value);
            }
        }
    }

    return NULL;
}
void calculate_print_stats(double times[], int runs)
{ // Calculate the mean of the times
    double sum = 0;
    for (int i = 0; i < runs; i++)
    {
        sum += times[i];
    }
    double mean = sum / runs;

    // Calculate the standard deviation of the times
    double sum_sq_diff = 0;
    for (int i = 0; i < runs; i++)
    {
        sum_sq_diff += (times[i] - mean) * (times[i] - mean);
    }
    double std_dev = sqrt(sum_sq_diff / (runs - 1));

    double required_n_root = (100 * 1.96 * std_dev) / (5 * mean);
    double req_n = required_n_root * required_n_root;

    printf("\nMean time taken for serial operations: %f seconds\n", mean);
    printf("Standard deviation of time taken: %f seconds\n", std_dev);
    printf("Required iterations (n): %f \n", req_n);
}

int main()
{
    printf("CASE 1: Member Operations:%f, Insert Operations:%f, Delete Operations:%f \n", mMember, mInsert, mDelete);
    // Seed the random number generator
    srand((unsigned int)time(0));

    // Generate the exact operations sequence
    char operations[m];
    generate_operations(operations);

    // ---------- SERIAL TEST ----------
    LinkedList serialList;
    init_list(&serialList);
    double serial_times[runs]; // Array to store times for each run

    printf("Serial Test:");

    for (int run = 0; run < runs; run++)
    {
        // Populate the serial list
        for (int i = 0; i < n; i++)
        {
            int value = rand() % 65536;
            if (!member(&serialList, value))
            { // TODO: If value is in the list
                // dont skip you need to retry
                insert(&serialList, value);
            }
        }

        // Start timing for the serial test
        clock_t start_serial = clock();
        test_serial_linked_list(&serialList, operations);
        clock_t end_serial = clock();
        double time_serial = ((double)(end_serial - start_serial)) / CLOCKS_PER_SEC;

        serial_times[run] = time_serial;
        // printf("\nTime taken for serial operations: %f seconds\n", time_serial);

        // Free the serial linked list
        free_list(&serialList);
    }

    calculate_print_stats(serial_times, runs);

    // ---------- MUTEX-BASED TEST ----------
    init_mutex_list(&mutexList);

    int thread_counts[] = {1, 2, 4, 8};

    for (int idx = 0; idx < 4; idx++)
    {
        int num_threads = thread_counts[idx];
        pthread_t threads[num_threads];
        int ops_per_thread = m / num_threads;

        printf("\nMutex-Based Test: Thread Count %f ", (double)num_threads);
        double mutex_times[runs]; // Array to store times for each run

        for (int run = 0; run < runs; run++)
        {
            // Populate the mutex-based list
            for (int i = 0; i < n; i++)
            {
                int value = rand() % 65536;
                if (!mutex_member(&mutexList, value))
                {
                    mutex_insert(&mutexList, value);
                }
            }

            // Start timing for the mutex-based test
            clock_t start_mutex = clock();

            // Create threads and pass the portion of the operations array to each
            for (int i = 0; i < num_threads; i++)
            {
                int start = i * ops_per_thread;
                int end = (i == num_threads - 1) ? m : (i + 1) * ops_per_thread;

                void *args[3] = {operations, &start, &end};
                pthread_create(&threads[i], NULL, mutex_thread_operations, args);
            }

            // Wait for all threads to finish
            for (int i = 0; i < num_threads; i++)
            {
                pthread_join(threads[i], NULL);
            }

            clock_t end_mutex = clock();
            mutex_times[run] = ((double)(end_mutex - start_mutex)) / CLOCKS_PER_SEC;

            // Free the mutex-based linked list
            free_mutex_list(&mutexList);
        }
        calculate_print_stats(mutex_times, runs);

        // ---------- RWLOCK-BASED TEST ----------

        printf("\nRWLock-Based Test: Thread Count %f ", (double)num_threads);
        init_rwlock_list(&rwlockList);

        double rw_times[runs]; // Array to store times for each run

        for (int run = 0; run < runs; run++)
        {
            // Populate the read-write lock-based list
            for (int i = 0; i < n; i++)
            {
                int value = rand() % 65536;
                if (!rwlock_member(&rwlockList, value))
                {
                    rwlock_insert(&rwlockList, value);
                }
            }

            // Start timing for the read-write lock-based test
            clock_t start_rwlock = clock();

            // Create threads and pass the portion of the operations array to each
            for (int i = 0; i < num_threads; i++)
            {
                int start = i * ops_per_thread;
                int end = (i == num_threads - 1) ? m : (i + 1) * ops_per_thread;

                void *args[3] = {operations, &start, &end};
                pthread_create(&threads[i], NULL, rwlock_thread_operations, args);
            }

            // Wait for all threads to finish
            for (int i = 0; i < num_threads; i++)
            {
                pthread_join(threads[i], NULL);
            }

            clock_t end_rwlock = clock();
            rw_times[run] = ((double)(end_rwlock - start_rwlock)) / CLOCKS_PER_SEC;

            // Free the read-write lock-based linked list
            free_rwlock_list(&rwlockList);
        }
        calculate_print_stats(rw_times, runs);
    }
    return 0;
}