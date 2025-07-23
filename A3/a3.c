#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define NUM_PROCESSES 15
#define NUM_STOCK_ITEMS 6

int stock[NUM_STOCK_ITEMS] = {11, 20, 5, 1, 8, 12};

char *processType[NUM_PROCESSES];
int processStockSelection[NUM_PROCESSES];
int processStockQty[NUM_PROCESSES];
char *processStatus[NUM_PROCESSES];

pthread_mutex_t mutexes[NUM_STOCK_ITEMS];

typedef struct {
    int processNum;
} ThreadArgs;

void populateProcesses(bool randomize);
void *processTransactionIn(void *arg);
void *processTransactionOut(void *arg);
void processTransactions();

int main() {
    populateProcesses(false); // Set to false to use hardcoded example processes

    printf("=== Initial Process List ===\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Process %d: Type '%s', Stock Item '%d', Qty '%d', Status '%s'\n",
               i, processType[i], processStockSelection[i], processStockQty[i], processStatus[i]);
    }

    processTransactions();

    printf("\n=== Final Stock Levels ===\n");
    for (int i = 0; i < NUM_STOCK_ITEMS; i++) {
        printf("Item %d: %d\n", processStockSelection[i], stock[i]);
    }
    printf("\n");

    return 0;
}

void getMutex(int itemIndex) {
    pthread_mutex_lock(&mutexes[itemIndex]);
}

void releaseMutex(int itemIndex) {
    pthread_mutex_unlock(&mutexes[itemIndex]);
}

void populateProcesses(bool randomize) {
    if (randomize) {
        srand(time(NULL));

        for (int i = 0; i < NUM_PROCESSES; i++) {
            processType[i] = (rand() % 2 == 0) ? "in" : "out";
            processStockSelection[i] = rand() % NUM_STOCK_ITEMS;
            processStockQty[i] = (rand() % 15) + 1;
            processStatus[i] = "ready";
        }
    } else {
        // Hardcoded processes based on the assignment specification page example
        processType[0] = "out"; processStockSelection[0] = 0; processStockQty[0] = 5; processStatus[0] = "ready";
        processType[1] = "out"; processStockSelection[1] = 3; processStockQty[1] = 3; processStatus[1] = "ready";
        processType[2] = "out"; processStockSelection[2] = 1; processStockQty[2] = 10; processStatus[2] = "ready";
        processType[3] = "in"; processStockSelection[3] = 2; processStockQty[3] = 3; processStatus[3] = "ready";
        processType[4] = "in"; processStockSelection[4] = 3; processStockQty[4] = 8; processStatus[4] = "ready";
        processType[5] = "out"; processStockSelection[5] = 5; processStockQty[5] = 10; processStatus[5] = "ready";
        processType[6] = "in"; processStockSelection[6] = 4; processStockQty[6] = 4; processStatus[6] = "ready";
        processType[7] = "in"; processStockSelection[7] = 0; processStockQty[7] = 3; processStatus[7] = "ready";
        processType[8] = "in"; processStockSelection[8] = 1; processStockQty[8] = 8; processStatus[8] = "ready";
        processType[9] = "out"; processStockSelection[9] = 5; processStockQty[9] = 5; processStatus[9] = "ready";
        processType[10] = "in"; processStockSelection[10] = 2; processStockQty[10] = 6; processStatus[10] = "ready";
        processType[11] = "in"; processStockSelection[11] = 4; processStockQty[11] = 5; processStatus[11] = "ready";
        processType[12] = "out"; processStockSelection[12] = 0; processStockQty[12] = 12; processStatus[12] = "ready";
        processType[13] = "in"; processStockSelection[13] = 3; processStockQty[13] = 5; processStatus[13] = "ready";
        processType[14] = "in"; processStockSelection[14] = 0; processStockQty[14] = 8; processStatus[14] = "ready";
    }
}

void *processTransactionOut(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int processNum = args->processNum;

    int itemIndex = processStockSelection[processNum];
    int requestedQty = processStockQty[processNum];

    printf("[Transaction %d] Starting OUT operation for Stock Item %d, Qty %d.\n", processNum, itemIndex, requestedQty);

    getMutex(itemIndex);

    if (stock[itemIndex] < requestedQty) {
        printf("[Transaction %d] PAUSE: Not enough stock for Stock Item %d (available: %d, requested: %d). Status set to WAITING.\n", processNum, itemIndex, stock[itemIndex], requestedQty);
        processStatus[processNum] = "waiting";
        releaseMutex(itemIndex);
        free(args);
        return NULL;
    } else {
        stock[itemIndex] -= requestedQty;
        printf("[Transaction %d] OUT operation completed for Item %d. New stock: %d. Status: DONE.\n", processNum, itemIndex, stock[itemIndex]);
        processStatus[processNum] = "done";
        releaseMutex(itemIndex);
        free(args);
        return NULL;
    }
}

void *processTransactionIn(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int processNum = args->processNum;

    int itemIndex = processStockSelection[processNum];
    int addedQty = processStockQty[processNum];

    printf("[Transaction %d] Starting IN operation for Item %d, Qty %d.\n", processNum, itemIndex, addedQty);

    getMutex(itemIndex);

    stock[itemIndex] += addedQty;
    printf("[Transaction %d] IN operation completed for Item %d. New stock: %d. Status: DONE.\n", processNum, itemIndex, stock[itemIndex]);
    processStatus[processNum] = "done";
    releaseMutex(itemIndex);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (strcmp(processStatus[i], "waiting") == 0 && processStockSelection[i] == itemIndex) {
            printf("[Transaction %d] Stock for Item %d increased. Marking WAITING transaction %d as READY.\n", processNum, itemIndex, i);
            processStatus[i] = "ready";
        }
    }

    free(args);
    return NULL;
}

void processTransactions() {
    for (int i = 0; i < NUM_STOCK_ITEMS; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    int incomingStockProcessesRemaining = 0;
    bool lastRun = false; // Keep track of when the last loop to attempt transactions happens
    pthread_t threads[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (strcmp(processType[i], "in") == 0) {
            incomingStockProcessesRemaining++;
        }
    }

    printf("\n=== Work Start ===\n");
    int loop_count = 0;

    while (true) { // Loop through list of transactions
        loop_count++;

        int current_processes_not_done = 0;
        int current_incoming_not_done = 0;
        int current_outgoing_waiting = 0;

        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (strcmp(processStatus[i], "done") == 0) {
                continue;
            }

            current_processes_not_done++;

            if (strcmp(processType[i], "in") == 0) {
                current_incoming_not_done++;
            } else if (strcmp(processType[i], "out") == 0 && strcmp(processStatus[i], "waiting") == 0) {
                current_outgoing_waiting++;
            }

            if (strcmp(processStatus[i], "ready") == 0 || strcmp(processStatus[i], "waiting") == 0) {
                ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
                args->processNum = i;

                int rc;
                printf("[Main] Attempting to launch Process %d (Type: %s, Status: %s).\n", i, processType[i], processStatus[i]);
                if (strcmp(processType[i], "in") == 0) { // Pass transaction to worker with incoming responsibility
                    rc = pthread_create(&threads[i], NULL, processTransactionIn, (void *)args);
                    if (incomingStockProcessesRemaining > 0) {
                        incomingStockProcessesRemaining--;
                    }
                } else if (strcmp(processType[i], "out") == 0) { // Pass transaction to worker with outgoing responsibility
                    rc = pthread_create(&threads[i], NULL, processTransactionOut, (void *)args);
                } else {
                    printf("[Main] Process %d has an invalid type: %s. Skipping.\n", i, processType[i]);
                    free(args);
                    continue;
                }
            }
        }

        if (current_processes_not_done == 0) {
            printf("\n...\nAll processes completed successfully!\n");
            break;
        }

        if (lastRun) {
            bool any_ready_processes_left = false;
            for (int i = 0; i < NUM_PROCESSES; i++) {
                if (strcmp(processStatus[i], "ready") == 0) {
                    any_ready_processes_left = true;
                    break;
                }
            }

            if (!any_ready_processes_left && current_outgoing_waiting > 0) {
                usleep(100000);
                printf("\n=== STOCK LIMITATION ===");
                printf("\nAll remaining transactions are outgoing and cannot be fulfilled due to insufficient stock.");
                printf("\nUnfulfilled transactions at simulation end:\n");
                for (int i = 0; i < NUM_PROCESSES; i++) {
                    if (strcmp(processStatus[i], "ready") == 0 || strcmp(processStatus[i], "waiting") == 0) {
                        printf("  - Transaction %d: Type '%s', Stock Item '%d', Qty '%d', Status '%s'\n",
                               i, processType[i], processStockSelection[i], processStockQty[i], processStatus[i]);
                    }
                }
                break;
            }
        }

        if (incomingStockProcessesRemaining == 0 && !lastRun) {
            printf("...\nAll incoming stock transactions have been launched.\n...\n");
            lastRun = true;
        }

        usleep(100000);
    }

    for (int i = 0; i < NUM_STOCK_ITEMS; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
    
    usleep(100000);
}

