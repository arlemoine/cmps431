#define _DEFAULT_SOURCE

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int core[2] = {-1, -1}; // -1 indicates that the core is available
int targetCore;
int burstTime[10] = {3, 20, 4, 5, 15, 5, 10, 5, 4, 5};
double elapsedTime[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

pthread_mutex_t coreLock;

typedef struct process {
    int number;
    int assignedCore;
} ThreadArgs;

void *runProcess(void *arg);
void timespecDiff(struct timespec *timeDiff, struct timespec *timeStop, struct timespec *timeStart);

int main() {
    pthread_t process[10];
    pthread_mutex_init(&coreLock, NULL); // Init the mutex lock for the cores
    struct timespec timeTotalStart;
    struct timespec timeTotalStop;
    struct timespec timeTotalDiff;

    clock_gettime(CLOCK_MONOTONIC, &timeTotalStart);

    for (int i = 0; i < 10; i++) {
        // Loop until a core is available for the next process
        while (true) {
            pthread_mutex_lock(&coreLock); // Lock the core variable
            if (core[0] == -1) {
                targetCore = 0;
                core[targetCore] = i;
                pthread_mutex_unlock(&coreLock); // Unlock the core variable
                break;
            }
            if (core[1] == -1) {
                targetCore = 1;
                core[targetCore] = i;
                pthread_mutex_unlock(&coreLock); // Unlock the core variable
                break;
            }
            pthread_mutex_unlock(&coreLock); // Unlock the core variable
            
            // Sleep function to slow down the loop
            usleep(100000);
        }

        // Allocate memory for thread's arguments and populate the struct
        ThreadArgs *args = (ThreadArgs *) malloc(sizeof(ThreadArgs));
        args->number = i;
        args->assignedCore = targetCore;

        // Using newly available core, start next process
        pthread_create(&process[i], NULL, runProcess, (void *) args); // Pass the process number and CPU burst time of the given process

        // If on the last process in queue, loop until both cores are available again
        if (i == 9) {
            while (true) {
                pthread_mutex_lock(&coreLock); // Lock the core variable
                if (core[0] == -1 && core[1] == -1) {
                    break;
                }
                pthread_mutex_unlock(&coreLock); // Unlock the core variable

                usleep(100000); // 0.1 sec
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &timeTotalStop);
    timespecDiff(&timeTotalDiff, &timeTotalStop, &timeTotalStart); // Calculate and store time elapsed
    double schedulerTotalTime = (double)timeTotalDiff.tv_sec + (double)timeTotalDiff.tv_nsec / 1000000000.0; // Store time elapsed in seconds (double)

    double sumProcessTime;
    for (int j = 0; j < 10; j++) {
        sumProcessTime += elapsedTime[j];
    };
    double avgProcessTime = sumProcessTime / 10;

    printf("...\nAll processes complete.\nAverage process time:\t%f sec\nTime elapsed:\t\t%f sec\n", avgProcessTime, schedulerTotalTime);   

    pthread_mutex_destroy(&coreLock);
    return 0;
}

void *runProcess(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int processNum = args->number;
    int assignedCore = args->assignedCore;
    int burstDuration = burstTime[processNum];

    printf("STARTING: Process %d on core %d, burstTime: %d\n", processNum, assignedCore, burstDuration);   

    // Store time variables
    struct timespec timeStart;
    struct timespec timeStop;
    struct timespec timeDiff;

    // Mark start time
    clock_gettime(CLOCK_MONOTONIC, &timeStart);

    // Simulate work for length of burst
    usleep(burstDuration * 100000);
    
    // Mark stop time and elapsed time
    clock_gettime(CLOCK_MONOTONIC, &timeStop);
    timespecDiff(&timeDiff, &timeStop, &timeStart); // Calculate and store time elapsed
    double totalSeconds = (double)timeDiff.tv_sec + (double)timeDiff.tv_nsec / 1000000000.0; // Store time elapsed in seconds (double)

    printf("\tCOMPLETED: Process %d on core %d, burstTime: %d, time: %f sec\n", processNum, assignedCore, burstDuration, totalSeconds);    
 
    elapsedTime[processNum] = totalSeconds;
    
    // Signal the "core" as being free
    pthread_mutex_lock(&coreLock); // Lock the core variable
    core[assignedCore] = -1;
    pthread_mutex_unlock(&coreLock); // Unlock the core variable

    free(args);

    return NULL;
}

void timespecDiff(struct timespec *timeDiff, struct timespec *timeStop, struct timespec *timeStart) {
    // Subtract timeStart from timeStop
    if (timeStop->tv_nsec < timeStart->tv_nsec) {
        // Used if nanoseconds in timeStop are less than timeStart, borrowing from seconds
        timeDiff->tv_sec = timeStop->tv_sec - timeStart->tv_sec - 1;
        timeDiff->tv_nsec = 1000000000L + timeStop->tv_nsec - timeStart->tv_nsec;
    } else {
        // No borrow needed
        timeDiff->tv_sec = timeStop->tv_sec - timeStart->tv_sec;
        timeDiff->tv_nsec = timeStop->tv_nsec - timeStart->tv_nsec;
    }
}
