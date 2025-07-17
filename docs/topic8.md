---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: Synchronization
topicDate: 2025-07-03
---
# Synchronization
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Intro

A **cooperating process** is one that can affect or be affected by other processes executing in the system. Cooperating processes can either directly share a logical address space (that is, both code and data) or be allowed to share data only through shared memory or message passing. Concurrent access to shared data may result in data inconsistency. In this chapter, we discuss various mechanisms to ensure the orderly execution of cooperating processes that share a logical address space, so that data consistency is maintained.

**Chapter objectives**
- Describe the critical-section problem and illustrate a race condition.
- Describe hardware solutions to the critical-section problem using memory barriers, compare-and-swap operations, and atomic variables.
- Explain how mutex locks, semaphores, monitors, and condition variables can be used to solve the critical-section problem.
- Evaluate tools that solve the critical-section problem in low-, moderate-, and high-contention scenarios.

**Study material**
6. Process Synchronization
   6.1 Background  
   6.2 The critical-section problem  
   6.3 Peterson's solution  
   6.4 Hardware support for synchronization  
   6.5 Semaphores  
   6.5.X  [Mutex locksLinks to an external site.](https://www.tutorialspoint.com/mutex-locks) (extra material provided-not in book)   6.6 Classic problems of synchronization  
   6.7 Monitors  
   6.8 Synchronization Examples  
   6.10 Summary  
            Practice exercises


## Race conditions

A **race condition** occurs when multiple processes try to access and manipulate the same data concurrently. 

The **two-phase locking protocol** is a well-known protocol to address race conditions. The idea is that each process must be able to put a lock on all of the resources that it needs before it is allowed to do its job so that those resources aren't modified by other processes in the middle of use. This produces **serializability**, which means that the result of a parallel execution is the same with what would be produced by a serial execution schedule.

## Critical section

The **critical section** of a process is the segment of its code in which the process may be accessing/updating data which is shared with at least one other process. No two processes can be executing in the critical section at the same time. Thus, we design a protocol which addresses the **critical section problem**.

One such way of implementation is a permission-based system where the process requests permission to enter its critical section by way of the **entry section**. When it enters, it locks that critical section from being altered by other processes. Then, it enters the **exit section** where it unlocks the critical section for use by other processes. The general structure would look like this:

```
while(true) {
	entry section # Locks the resource
	critical section # Works on the resource
	exit section # Unlocks the resource
	remainder section
}
```

A solution to the **critical section problem** must satisfy the following requirements. If the critical section is empty and there are processes that genuinely want to enter, then the logic of the **critical section entry protocol** must ensure that:

1. Only processes that are **currently trying to enter** (i.e., they've finished their non-critical work and are executing the entry section code) can influence the outcome of who gets in.
2. No process that is **not trying to enter**, or is stuck/crashed/doing unrelated work, can prevent a waiting process from entering.
3. A decision will be made in a **finite amount of time**, allowing one of the genuinely waiting processes to enter.

## Peterson's Solution

**Peterson's Solution** is a classic software-based solution for the **Critical Section Problem** for two processes. It ensures that only one process can enter its critical section at a time.

It relies on two shared data items:
- `int turn;`: Indicates which process has permission to enter its critical section (`turn = i` means Process Pi​ can enter).
- `boolean flag[2];`: An array where `flag[i] = true` means Process Pi​ is ready to enter its critical section.

### How it Works (for Process P0 wanting to enter):

1. **P0 sets `flag[0] = true`** (declares its intent to enter).
2. **P0 sets `turn = 1`** (gives priority to the _other_ process, P1, as a "courtesy").
3. **P0 then waits** (`while (flag[1] == true && turn == 1)`)
4. Once the loop condition is false, P0 enters its **critical section**.
5. After exiting its critical section, **P0 sets `flag[0] = false`** (indicates it's no longer interested).

This "courtesy" mechanism ensures that eventually, `turn` will settle, allowing only one process to proceed.

### Why it Might Fail on Modern Systems

Peterson's Solution is **not guaranteed** to work on modern computer architectures. This is because:
- **Processor and Compiler Optimizations:** Modern CPUs and compilers can reorder read and write operations to improve performance, even if they have no direct dependencies.
- **Impact on Shared Data:** While reordering doesn't affect single-threaded programs, it can lead to **inconsistent or unexpected results** in multi-threaded applications with shared data.

#### Example of Reordering Issue:

Consider `boolean flag = false;` and `int x = 0;` shared between two processes:

|Process P1|Process P2|
|---|---|
|`while (!flag);` (busy-wait for flag)|`x = 100;`|
|`print x;`|`flag = true;`|

If P2's operations are reordered, `flag = true;` might execute _before_ `x = 100;`. In that case, P1 could see `flag` as `true` and print `x` _before_ `x` has been set to `100`, leading to incorrect output.

## Mutex Locks

Type of lock. Also referred to as **mutual exclusion locks**. Synchronization basic functions used to prevent simultaneous possession of resources that are shared by numerous threads or processes. Limits the number of threads/processes that can simultaneously acquire the lock. The thread/process has to first obtain the mutex lock for something that is shared before gaining access. When waiting for the mutex lock to become available, it is placed in a state of waiting. After acquiring the lock, the thread/process is able to use the resource that has been shared. When finished, it releases the lock so that different threads/processes can take possession of it. 

### Components

- **Mutex variable**: Variable representing the lock. Data structure maintaining state of lock, allowing threads/processes to acquire and release it.
- **Lock acquisition**: The gaining of ownership of the mutex lock by a requesting thread/process. Until acquisition, the thread/process stays in a waiting state. 
- **Lock release**: Release of the mutex lock by the thread/resource using it, allowing the lock to be used by other threads/resources.

### Psuedo-code

```
main() {
	while (true) {
		acquire lock
			critical section
		release lock
		remainder section
	}
}

// Acquires the lock
acquire() {
	while (!available) 
	    ; # busy wait 
	available = false;
} 

// Releases the lock
release() {
	available = true; 
}
```

### Types

- **Recursive mutex**: Enables multiple acquisitions of the lock by a single thread/process without blocking the thread/process, maintaining appropriate traversal and synchronization. 
- **Error-checking mutex**: Makes a guarantee that an application or procedure doesn't take over a mutex lock it presently already holds. 
- **Times mutex**: Only gives the thread/process a certain amount of time to access what it is trying to access. If the thread/process is in wait due to another thread/process and the time expires, the lock is released. 
- **Priority inheritance mutex**: Temporarily speeds up the low-priority task that holds a needed resource, specifically so that a waiting high-priority task can get that resource sooner.
- **Read-write mutex**: Allows multiple "readers" to access a resource at the same time, but only one "writer" at a time, blocking all readers and other writers during a write operation.

### Use cases

Mutex locks are essential for managing shared resources in programs where multiple tasks run at once. They primarily:
- **Protect Shared Resources:** Ensure only one task accesses a shared resource (like data or a file) at a time, preventing errors and corruption.
- **Define Critical Sections:** Mark sections of code where only one task can execute at a time, safeguarding data integrity.
- **Enable Synchronization:** Help tasks coordinate their actions and access shared resources in an orderly way, ensuring operations happen correctly.
- **Aid Deadlock Avoidance:** When used correctly, they can prevent situations where tasks get stuck waiting for each other indefinitely.

## Spinlock

A **spinlock** is a type of lock that, when unable to acquire a resource, causes the waiting thread to **"busy-wait"** (continuously check in a tight loop) instead of putting the thread to sleep. It's used for **very short critical sections** where the overhead of context switching (like with a traditional mutex) would be greater than the time spent busy-waiting. While efficient for brief waits, it **wastes CPU cycles** if the lock is held for long.

```
acquire() {
	while (!available) 
	    ; # busy wait 
	available = false;
} 
```

## Lock contention

A lock is considered **contended** if a thread blocks while trying to acquire the lock. It is considered **uncontended** if the lock is available when the thread is attempting to acquire it. 

Contention locks can experience either **high contention** (large number of threads attempting acquire) or **low contention**. 

## Semaphores

A **semaphore** is an integer variable used as a signaling mechanism in concurrent programming. It acts like a **counter for available resources** combined with a **waiting queue** for processes that need those resources.

It's controlled by two atomic (uninterruptible) operations:

 1. **`wait()` (or `P`, `acquire`):** Decrements the semaphore value. If the value becomes negative or zero (no resources available), the calling process is **suspended** (put to sleep) and added to the semaphore's waiting queue.

```
wait(semaphore S) {
	S.value--;
	if (S.value < 0) {
		add this process to S.queue;
		sleep(); // Suspend process
	}
}
```

2. **`signal()` (or `V`, `release`):** Increments the semaphore value. If the value is now zero or positive (resources available or a process was waiting), one process from the waiting queue is **woken up** and moved to the ready state.

```
signal(semaphore S) {
    S.value++;
    if (S.value <= 0) { // If there were processes waiting
        remove a process P from S.queue;
        wakeup(P); // Resume process
    }
}
```

### Types and Use Cases

1. **Binary Semaphore (Mutex-like):**
    - Value is `0` or `1`.
    - Used primarily for **mutual exclusion** (protecting a critical section).
    - **Initialization:** Typically `1`.
    - **Example:** Only one process can be inside a code block.

```
semaphore mutex = 1;

Process A:
    wait(mutex);
    // Critical Section
    signal(mutex);
```

2. **Counting Semaphore:**
- Value can be any non-negative integer.
- Used to control access to a **pool of multiple identical resources**.
- **Initialization:** `N` (number of available resources).
- **Example:** Limit concurrent access to 5 printers.

```
semaphore printers = 5;

Process B:
    wait(printers);
    // Use a printer
    signal(printers);
```

3. **Synchronization/Ordering:**
- Used to ensure one event happens _after_ another, even across different processes.
- **Initialization:** Typically `0`.
- **Example:** `Statement S2` must run only after `Statement S1` completes.

```
semaphore synch = 0;

Process P1:
    S1;
    signal(synch); // Signal S1 is done

Process P2:
    wait(synch); // Wait for S1 to be done
    S2;
```

### Key Distinction (vs. Spinlock/Mutex)

Unlike **spinlocks** which busy-wait (continuously consume CPU cycles while checking), semaphores typically **suspend (sleep)** waiting processes. This frees the CPU for other tasks, making them more efficient for longer waits or when CPU cycles are precious. While binary semaphores can act like mutexes, semaphores are generally more versatile due to their counting and synchronization capabilities.

## Monitors

One way to outsource synchronization. Also provides ability to set conditions for sync. 

A monitor type is an **abstract data type** (ADT), encapsulating data with a set of functions to operate on that data. The monitor includes programmer-defined operations which house mutex within the monitor. The monitor type also declares variables for the state of the instance as well as the bodies of functions that operate on the variables. 

It's essentially a class with private methods and a queue. Processes that want to enter a monitor must wait in the queue. The process is only allowed to enter into the monitor when there are no processes actively in the monitor and running any of the private methods.

A programmer who needs to write a tailor-made synchronization scheme can define one or more variables of type *condition*. Each condition variable has its unique block queue which contains the processes waiting on that condition.

The only operations that can be invoked on a condition variable are wait() and signal(). The operation:
`x.wait()`; 
means that the process invoking this operation goes into wait and is suspended until another process invokes:
`x.signal()`; 
Many processes may be associated with a particular condition x.  Processes waiting on the condition are suspended. The x.signal() operation resumes exactly one suspended process. If no process is suspended, the signal() operation has no effect.  

When a process P issues a signal, it immediately yields and the process Q that is supposed to wake up is immediately resumed.

### Advantages

Makes parallel programming easier and less error prone than something like semaphores. Further, processes will not be deadlocked due to failing to release resources, as can happen with a semaphore if a bug prevents the semaphore from being released.

### Disadvantages

Have to be implemented as part of the program, allowing compiler to generate code for them.

 **The following problems can occur:
- A process might access a resource without first gaining access permission to the resource.
- A process might never release a resource once it has been granted access to the resource.
- A process might attempt to release a resource that it never requested.
- A process might request the same resource twice (without first releasing the resource).
  
Previously, we had to worry about the correct use of semaphores. Now, we have to worry about the correct use of higher-level programmer-defined operations, with which the compiler can no longer assist us. We must also ensure that an uncooperative process does not simply ignore the mutual-exclusion gateway provided by the monitor and try to access the shared resource directly, without using the access protocols.

### Supported languages

This includes but is not limited to Java, C#, Visual Basic, Ada, and Euclid. 

### Using semaphores for implementation

Monitor may use a binary semaphore mutex (variable initialized to 1). A process here must execute wait(mutex) before entering the monitor and must execute signal(mutex) after leaving the monitor.

### Pseudocode

```
R.acquire(t);
   …
   access the resource;
   …
 R.release(); 
 
monitor ResourceAllocator { boolean busy;
  condition x;

 void acquire(int time) {
   if (busy) x.wait(time);
   busy = true;
 }
 
 void release() {
   busy = false;
   x.signal();
 }
 
 initialization_code() { busy = false; } } 
```

## Deadlocks

The implementation of a semaphore with a waiting queue may result in a situation where two or more processes are waiting indefinitely for an event that can be caused only by one of the waiting processes. This is termed **deadlock**.

A related concept is that of **liveness**: A set of properties that a system must satisfy to ensure that processes make progress during their execution life cycle. An infinite loop is a simple example of a liveness failure.

Example:  Say we have three processes: L, M, and H, with priorities ordered: L < M < H. Assume that process H requires a semaphore for resource S, which is currently being held by process L. 

Ordinarily, process H would wait for L to finish using resource S. However, now suppose that process M becomes runnable, thereby preempting process L. So, A process M of intermediate priority may get to run while process H of high priority is waiting on L of very low priority, which has been preempted by M. Indirectly, a process with a lower priority—process M—has affected how long process H must wait for L to relinquish resource S.  

This liveness problem is known as priority inversion, and it can occur only in systems with more than two priorities. Typically, priority inversion is avoided by implementing a priority-inheritance protocol. According to this protocol, all processes that are accessing resources needed by a higher-priority process inherit the higher priority until they are finished with the resources in question. When they are finished, their priorities revert to their original values. A priority-inheritance protocol would allow process L to temporarily inherit the priority of process H, thereby preventing process M from preempting its execution. When process L had finished using resource S, it would relinquish its inherited priority from H and assume its original priority. Because resource S would now be available, process H—not M—would run next.
