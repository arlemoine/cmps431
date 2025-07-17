---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: Threads
topicDate: 2025-06-19
---
# Threads
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Study material

4. Threads 

   4.1 Overview  
   4.2 Multithreading models  
   4.3 Thread libraries  
   4.4 Threading issues  
   4.5 Operating-system examples  
   4.6 Summary  
          Practice exercises

## Introduction

A **thread** allows for an application to perform several CPU-intensive tasks in parallel across multiple CPU cores. It is a segment of a process or an execution unit that is part of a process. Benefits include responsiveness, resource sharing, economy, and scalability. 

A thread is defined as a basic unit of CPU utilization. It consists of a thread ID, a program counter (PC), a register set, and a stack. 

States of a process are new, ready, running, waiting, terminated, and suspended. 

States of a thread are running, ready, and blocked. 

A process is a collection of code, memory, data, and other resources. A thread is a sequence of code that is executed within the scope of the process. 

There are user level threads and kernel level threads. User level threads are bound to kernel-level threads to get carried out. 

## Amdahl's Law

Discusses to what degree more resources leads to more performance by identifying potential performance gains from adding additional computing cores to an application that has both serial and parallel components. As N approaches inifity, the speedup converges to 1/S. 

$$\text{speedup} \leq \frac{1}{S + \frac{1-S}{N}}$$

As resources keep increasing, speedup increases at reduced rates. 

![[amdahls_law.jpg]]

## Universal Scalability Law

Combines Amdahl's Law and Gustafson's Law, stating that throughput can decline due to the amount of processing required for process management as the number of processing cores increases. 

![[universal_scalability_law.jpg]]

## Multithreading models

### Many-to-one

Many user-level threads to one kernel thread. A thread library manages threads in user space to maintain efficiency. Only one thread can access the kernel at a time so **multiple threads cannot run in parallel on multicore systems.** 

An example of a system which uses this model is **Solaris systems**, utilizing a thread library called **green threads** in the early versions of Java. 

### One-to-one

Maps each user thread to a kernel thread, providing more concurrency than the many-to-one model by allowing another thread to run when a thread makes a blocking system call. Allows **multiple threads to run in parallel on multiprocessors**. 

A downside to this system is having double the required threads by creating a kernel thread for each user thread. 

Linux and Windows systems use this model. 

### Many-to-many

Maps many user threads to a less than or equal to number of kernel threads. 

Some systems which use this include DigitalUNIX and Solaris. 

## Thread libraries

Provides an API to developers for creating/managing threads. 

## Implicit threading

Relates to transferring creation and management of threading from developers to compilers and run-time libraries. There are 4 major ways to implement this: thread pools, fork-join, OpenMP, and Grand central dispatch. 

These strategies require developers to identify tasks that can run in parallel and let the libraries determine the specific details of thread creation and management. A **task** is usually written as a function which the run-time library maps to a separate thread. 

The advantage to this approach is that developers need only identify parallel tasks and leave thread management up to the system. 

### Thread pools

Create a number of threads at startup and place them in a pool where they sit and wait for work. Tasks are sent to the thread pool and assigned to an available thread or waits in a queue. 

### Fork-join

Main parent thread creates (or forks) one or more child threads and waits for them to terminate. Then, it retrieves or combines their results.

The number of threads are determined by the library which manages them. 

### OpenMP

This is a set of compiler directives and an API for programs writtin in C, C++, or FORTRAN, providing support for parallel programming in shared-memory environments. 

Developers identify parallel regions as blocks of code that may run in parallel and inserts compiler directives into their code to mark parallel regions. 

### Grand central dispatch

Technology developed by Apple for macOS and iOS operating systems. It's a combination of a run-time library, an API, and language extensions that allow developers to identify sections of code (tasks) to run in parallel. 

GCD schedules tasks for run-time execution by placing them in 1 of 2 dispatch queues: serial or concurrent. 

### Thread building blocks

Thread building blocks (TBB) is a template library by intel that supports designing parallel applications in C++. 

## Signal handling

Signals are used in UNIX systems to notify a process that an event has occurred. They are received either:
- Syncronously: Receiving process is also the source.
- Asyncronously: Source is external to the receiver. 

Signals are handled by either:
- Default signal handler
- User-defined signal handler

## Thread cancellation

Refers to terminating a target thread before it has competed. There are two different types of cancellation:
- Asyncronous cancellation: One thread immediately terminates the target thread. OS may not be able to reclaim all allocated resources. 
- Deferred calcellation: Target thread periodically checks if it should terminate, allowing itself to terminate in an orderly fashion. 

## Thread-local storage

Thread-local storage (TLS) is the system by which a thread maintains its own copy of certain data. 

## Scheduler activations

Many systems place an intermediate structure **between the user and kernel threads**. This structure is known as a **lightweight process** (LWP), appearing to the user-thread library as a virtual processor on which the application can schedule a user thread to run. 

The scheme for communication between the user-thread library and the kernel is kown as **scheduler activation**. The kernel provides an application with a set of virtual processors (for LWPs) and the application can schedule user threads onto an available virtual processor. 

The kernel must inform an application about certain events via an **upcall**. Upcalls are handled by the thread library via an **upcall handler**. 

## Operating system examples

### Windows

A Windows application runs as a separate process and each process may contain one or more threads. Windows uses the one-to-one mapping where each user-level thread maps to an associated kernel thread. The general components of a thread include:
- Thread ID
- Register set representing status of processor
- Program counter
- User stack and kernel stack
- Private storage area for run-time libraries

The register set, stacks, and private storage area are known as the **context** of the thread. 

### Linux

Linux provides the fork() system call with the traditional functionality of duplicating a process. It also provides the clone() system call for creating threads. However, it does not distinguish between processes and threads and uses the term **task** instead of process or thread. 

With fork(), a new task is created along with a copy of all associated data structures of the parent process. 

With clone(), the one key difference is that instead of copying all data structures, the new task **points** to the data structures of the parent task, depending on the set of flags passed to clone(). 

## Notes

Two general strategies for creating multiple threads:
- **Asynchronous threading**: Parent creates child threads which execute concurrently and independently of the parent.
- **Syncronous threading**: Parent creates child threads and waits for all children to terminate before resuming. 

Example of using Pthread library (API): 
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; /* this data is shared by the thread(s) */
void *runner(void *param); /* threads call this function */

int main(int argc, char *argv[]) {
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */
	/* set the default attributes of the thread */
	pthread_attr_init(&attr);
	/* create the thread */
	pthread_create(&tid, &attr, runner, argv[1]);
	/* wait for the thread to exit */
	pthread_join(tid,NULL);
	
	printf("sum = %d\n",sum);
}

/* the thread will execute in this function */
void *runner(void *param) {
	int i, upper = atoi(param);
	sum = 0;
	
	for (i = 1; i <= upper; i++) {
		sum += i;
	}
	
	pthread_exit(0);
}
```

