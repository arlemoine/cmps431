---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: Deadlocks
topicDate: 2025-07-10
---
# Deadlocks
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Introduction

In this module, we describe methods which can be used to prevent or deal with deadlocks. Although some applications can identify programs that may deadlock, operating systems typically do not provide deadlock-prevention facilities, and it remains the responsibility of programmers to ensure that they design deadlock-free programs. Deadlock problems as well as other liveness failures are becoming more challenging as demand continues for increased concurrency and parallelism on multicore systems.

**Major topics**:
Explain how deadlock can occur when mutex locks are used.
Define the four necessary conditions that characterize deadlock.
Identify a deadlock situation in a resource allocation graph.
Evaluate the four different approaches for preventing deadlocks.
Apply the banker's algorithm for deadlock avoidance.
Apply the deadlock detection algorithm.
Evaluate approaches for recovering from deadlock.

**Module objectives**:
Explain how deadlocks can occur and how they are characterized
Articulate methods used to detect and prevent deadlocks as well as to methods for recovering from deadlocks
Explain and apply specific algorithms used for deadlock management

**Study material**
7. Deadlocks
   7.1 System model
   7.2 Deadlock characterization
   7.3 Methods for handling deadlocks
   7.4 Deadlock prevention
   7.5 Deadlock avoidance
   7.6 Deadlock detection
   7.7 Recovery from deadlock
   7.8 Summary
         Practice exercises

## Deadlocks

A **deadlock** is a situation where two or more threads wait indefinitely for events caused only by the other waiting threads. Typical resource usage pattern: `request → use → release`.

Examples of resource use via system calls:
- request() and release() of a device, 
- open() and close() of a file, 
- allocate() and free() memory system calls 
- wait() and signal() operations on semaphores 
- acquire() and release() of a mutex lock.

In a multithreaded Pthread program using POSIX mutex locks:
- pthread_mutex_init() initializes an unlocked mutex
- pthread_mutex_lock() acquires mutex
- pthread_mutex_unlock() releases mutex

### Necessary conditions of a deadlock

A system is in deadlock when all four of the following hold:
- **Mutual Exclusion**: At least one non-sharable resource.
- **Hold and Wait**: A thread holds resources while waiting for others.
- **No Preemption**: Resources cannot be forcibly taken.
- **Circular Wait**: A closed chain of threads exists where each waits for the next.

### Livelock

Similar to deadlock, but threads constantly change state without making progress. Can be mitigated by retrying failed operations after **randomly timed delays** (e.g., Ethernet backoff).

Whereas deadlock occurs when all threads are blocked waiting for an event that can be caused only by another thread in the set, livelock occurs when a thread **continuously attempts an action that fails** (like spinning). 

## Visualizing Deadlocks

**Resource Allocation Graph (RAG)**:

- No cycles → no deadlock.
- Cycle + 1 instance per resource → deadlock.
- Cycle + multiple instances → possible deadlock.

![[resource_allocation_graph.png]]

## Strategies for Handling Deadlocks

### 1. Ignore

Many systems simply ignore deadlocks (e.g., reboot if they occur rarely).

### 2. Prevention

Ensure at least one deadlock condition **cannot** hold:
- **Mutual Exclusion**: Make resources sharable where possible.
- **Hold and Wait**: Require threads to request all resources at once.
- **No Preemption**: Reclaim resources from waiting threads.
- **Circular Wait**: Impose a strict ordering on resource acquisition.

**Example lock ordering failure**:  

Dynamic locking like below can break static ordering and lead to deadlock.

```cpp
transaction(savings, checking)
transaction(checking, savings)
```

### 3. Avoidance

Requires knowledge of future resource needs.

#### Safe State

A state is **safe** if the system can satisfy all maximum needs without entering deadlock.

#### Resource Allocation Graph (extended)

Adds **claim edges** for possible future requests. A request is granted only if it doesn't form a cycle.

> Works only with **single instance** of each resource type.

#### Banker's Algorithm

Used when multiple instances exist. Each thread declares its **maximum need** in advance.

##### Key Data Structures

- `Available[m]`: Free instances of each resource type.
- `Max[n][m]`: Max demand of each thread.
- `Allocation[n][m]`: Current allocation.
- `Need[n][m] = Max - Allocation`

##### Safety Check

Uses `Work[m]` (copy of `Available`) and `Finish[n]`:

1. Find a thread `i` where `Finish[i] == false` and `Need[i] ≤ Work`.
2. Pretend it runs:  
    `Work += Allocation[i]`, `Finish[i] = true`.
3. Repeat until no changes.

If all `Finish[i] == true`, the state is **safe**.

### 4. Detection

Allows deadlock to occur, then detects and recovers.

#### Single Resource Instance

Use a **wait-for graph** (simplified RAG):
- Cycle → deadlock exists.
- Detection cost: **O(n²)**

#### Multiple Instances

Uses similar structures to Banker's Algorithm.

##### Detection Steps

1. Initialize:
    - `Work = Available`
    - `Finish[i] = false` if `Allocation[i] ≠ 0`, else `true`
2. Find `i` where `Finish[i] == false` and `Request[i] ≤ Work`
3. Simulate:
    - `Work += Allocation[i]`
    - `Finish[i] = true`
4. Repeat until no changes.

If any `Finish[i] == false`, those threads are deadlocked.  
Cost: **O(m × n²)**

### 5. Recovery

#### Termination

- **Abort all deadlocked threads**: Guaranteed but expensive.
- **Abort one at a time**: Lower impact, higher overhead.

**Victim selection** criteria:
- Thread priority
- Amount of work done/remaining
- Resource types used
- Potential resource reuse

#### Resource Preemption

Take resources from some threads to unblock others.

Issues:
- **Victim selection**
- **Rollback** (usually full restart)
- **Starvation**: Avoid repeated preemption of the same thread

## Other Notes

- **Lockdep (Linux)**: Runtime tool to detect improper lock acquisition order.
    
- **POSIX Mutex Locking**:

```c
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);
pthread_mutex_lock(&mutex);
// critical section
pthread_mutex_unlock(&mutex);
```
