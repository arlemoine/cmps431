---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: CPU Scheduling
topicDate: 2025-06-26
---
# CPU Scheduling
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Objectives

In this chapter, we use process scheduling when discussing general scheduling concepts and thread scheduling to refer to thread-specific ideas. We describe how a core is the basic computational unit of a CPU, and that a process executes on a CPU's core. 

**Chapter objectives:**
- Describe various CPU scheduling algorithms.
- Discuss the assessment of CPU scheduling algorithms based on scheduling criteria.
- Explain the issues related to multiprocessor and multicore scheduling.
- Describe various real-time scheduling algorithms.
- Apply modeling and simulations to evaluate CPU scheduling algorithms.

**Topics:**
5. CPU Scheduling
   5.1 Basic concepts
   5.2 Scheduling criteria
   5.3 Scheduling algorithms
   5.4 Thread scheduling
   5.5 Multi-processor scheduling
   5.6 Operating-system examples
   5.7 Algorithm evaluation
   5.8 Summary & Practice exercises

## Intro

Process execution consists of a cycle of alternating periods of CPU execution and I/O wait.

The **CPU scheduler** **selects** a process from the processes in memory that are ready to execute (ready queue) and **allocates** the CPU to that process. Important here is the **algorithm** used for selecting the process to be allocated to the CPU.

Processes in the ready queue are lined up waiting for a chance to run on the CPU. Their records in the queue are the process control blocks (PCBs). 

The **ready queue** can be implemented as:
- a FIFO queue
- a priority queue
- a tree
- or simply an unordered linked list

## Preemptive and non-preemptive scheduling

CPU-scheduling decisions may take place under the following four circumstances:

| Circumstance                             | Type                        | Example                                                                                           |
| ---------------------------------------- | --------------------------- | ------------------------------------------------------------------------------------------------- |
| Process **running** -> **waiting** state | Preemptive                  | As the result of an I/O request or an invocation of wait() for the termination of a child process |
| Process **running** -> **ready** state   | Preemptive or nonpreemptive | when an interrupt occurs                                                                          |
| Process **waiting** -> **ready** state   | Preemptive or nonpreemptive | at completion of I/O                                                                              |
| Process **terminates**                   | Preemptive                  | -                                                                                                 |

**Nonpreemptive**: Once the CPU has been allocated to a process, the process **keeps** the CPU until it releases it by either **terminating** or by **switching** to the waiting state. Also called **cooperative**.

**Preemptive**: Pause the process and allocate the CPU to another process. As noted above, case 2 and 3 allow for preemptive scheduling, but there is a choice. 

## Dispatcher

The **dispatcher** is the module that **gives control** of the CPU's core to the process selected by the CPU scheduler. This involves: 
- Switching context from one process to another
- Switching to user mode
- Jumping to the proper location in the user program  to resume that program

The dispatcher should be as fast as possible, since it is Invoked during every **context switch**. The time it takes for the dispatcher to stop one process and start another running is known as the **dispatch latency**.

On Linux, the command `vmstat` and `/proc` provides relevant statistics. Example:

>[!Example]
>This `vmstat 1 3` commandd provides 3 lines of output over a 1 second delay:
>```bash
>------cpu----- 
>24 
>225
>339
>```
>The first line gives the average number of context switches over 1 second since the system booted, and the next two lines give the number of context switches over two 1-second intervals. Since this machine booted, it has averaged 24 context switches per second. And in the past second, 225 context switches were made, with 339 context switches in the second prior to that.

## Scheduling criteria

The following criteria are generally used for comparing CPU-scheduling algorithms:
- **CPU utilization**. Keeping the CPU as busy as possible (range is 0 to 100 percent). Think weight.
- **Throughput**. The number of processes that are completed per time unit. Think volume.
- **Turnaround time**. The interval from the time of submission of a process to the time of completion. Turnaround time is the sum of the periods spent waiting in the ready queue, executing on the CPU, and doing I/O. Think ETA. 
- **Waiting time**. Waiting time is the sum of the periods spent waiting in the ready queue. 
- **Response time**. The time from the submission of a request until the first response is produced. This measure, is the time it takes to start responding, not the time it takes to output the response.**

It is desirable to maximize CPU utilization and throughput and to minimize turnaround time, waiting time, and response time. In most cases, we optimize the average measure. However, under some circumstances, we prefer to optimize the minimum or maximum values rather than the average.

## Scheduling algorithms

### First come first served (FCFS)

Non-preemptive approach. The process that requires the CPU first is allocated it first. Implementation easily managed with a queue. 

**Advantages**:
- Least complex method and easy to implement.
- Useful for simple systems. 

**Disadvantages**:
- Creates the **convoy effect**, where a long running process delays many short, quick processes, causing a "traffic jam" of waiting tasks. 

### Shortest job first scheduling (SJF)

When the CPU is available, it assigns the smallest process to the next CPU burst. If there are two processes that meet this requirement, tie-breaker is handled by FCFS logic. 

Optimal for minimizing average waiting time over all other schemes. However, it cannot be implemented at the level of CPU scheduling because there is no way to know the length of the next CPU burst. 

This is how we attempt to predict this CPU burst length. It is generally predicted as an exponential average of the measured lengths of previous CPU bursts:
$$ \begin{align}
t_{n+1}=\alpha t_{n}+(1-\alpha)t_{n} \\
\end{align}
$$
where 
	$t_{n}$ is the length of the $n_{th}$ CPU burst 
	$t_{n+1}$ is the predicted length of the next CPU burst
	$t_{0}$ is either a constant or the overall system average
	$0\leq\alpha\leq1$

**Example**:
![[sjf_example.png]]

### Round robin scheduling (RR)

Similar to FCFS, but preemption is utilized to allow switching between processes at a time rate referred to as a **time slice** or **time quantum**. This time slice is typically 10 to 100 msec in length. The ready queue is implemented as a circular queue and the scheduler goes around the ready queue, giving each process up to 1 time quantum before it passes the core to the next process. 

The scheduled process either runs to completion and voluntarily gives up its core or it is interrupted at 1 time quantum.

The performance of the RR is determined by the size of the time quantum. If it is too large, the RR policy is the same as FCFS. If it is too small, there are too many context switches and associated switching overhead. 

### Priority scheduling

Each process is given a priority either internally or externally. Some internal measurements include things like time limits, memory requirements, number of open files, or ratio of average I/O burst to average CPU burst. 

### Multilevel queue scheduling

Separate queues based on priority level. Alternatively, a multilevel feedback queue allows processes to move between priority level queues. 

## Contention scope

User-level threads are mapped to kernel-level threads via [[topic6#Scheduler activations|LWPs]]. **Process-contention scope** (PCS) refers to user-level threads competing for LWPs. **System-contention scope** (SCS) refers to kernel-level threads competing for the CPU. 

Systems using the one-to-one model, such as Windows and Linux schedule threads using only SCS. Typically, PCS is done according to priority - the scheduler selects the runnable thread with the highest priority to run. 

## Multiprocessing

The term multiprocessing applies to the following situations:
- Multicore CPUs
- Multithreaded cores
- NUMA systems (non-uniform memory access)
- Heterogeneous multiprocessing

### Asymmetric multiprocessing

System of operation by which one processor (the master server) is responsible for all scheduling decisions, I/O processing, and other system activities. The other processors execute only user code. With this system, the master server may become a bottleneck, depending on the use. 

### Symmetric multiprocessing (SMP)

System of operation which is the standard approach for supporting multiprocessors. Each processor is self-scheduling. For a given processor, it examines the ready queue and selects a thread to run. There may be one ready queue shared by all processors or one ready queue per processor. 

### Hardware threads

A **memory stall** is the time spent by a processor waiting for data to become available from memory. This time can be relatively significant - up to 50% of its waiting time. This occurs for two reasons: 

1. Modern CPU speeds are much faster than memory speeds. 
2. A cache miss could occur (accessing data not cached in memory). 

This is remedied by the use of multithreaded processing cores which utilize multiple **hardware threads** assigned to each core. If one hardware thread stalls while waiting for memory, the core can switch to another physical thread. For Intel processors, this is called hyper-threading. 

### Load balancing

**Load balancing** attempts to keep the workload evenly distributed across symmetric multiprocessing (SMP) system. This is only the case with individual ready queues, as having a common ready queue would by design balancing loading between cores. With load balancing, there are two general approaches: 

1. **Push migration**:  A specific task periodically checks the load on each processor and - if it finds an imbalance - evenly distributes the load by pushing threads from more busy to less busy processors. 
2. **Pull migration**:  An idle processor pulls a waiting task from a busy processor. 

### Processor affinity

When a thread migrates to another processor, the contents of cache memory must be invalidated for the first processor, and the cache of the second processor must be repopulated. 

**Processor Affinity** is the tendency or preference for a process or thread to run on the same CPU core it was previously using, primarily because its data and instructions are already loaded into that core's fast cache memory, which improves performance by avoiding the cost of re-populating the cache on a different core.

### Event latency

Event latency is the amount of time that elapses from when an event occurs to when it is serviced. Usually, different events have different latency requirements. For example, the latency requirement for an antilock brake system might be 3 to 5 milliseconds. Two types of latencies affect the performance of real-time systems:
- Interrupt latency (due to an interrupt)
- Dispatch latency (due to the scheduling dispatcher which stops one process and starts another)

## Prioritization

### Rate-monotonic scheduling

The **rate-monotonic algorithm** schedules periodic tasks using a static priority policy with preemption. If a lower-priority process is running and a higher-priority process becomes available to run, it will preempt the lower-priority process. Upon entering the system, each periodic task is assigned a priority inversely based on its period. 

The shorter the period, the higher the priority; the longer the period, the lower the priority. 

The rationale behind this policy is to assign a higher priority to tasks that require the CPU more often. Furthermore, rate-monotonic scheduling assumes that the processing time of a periodic process is the same for each CPU burst. That is, every time a process acquires the CPU, the duration of its CPU burst is the same.

### Earliest deadline first scheduling (EDF)

Assigns priority dynamically according to deadline. 

### Proportional share scheduling

Proportional share schedulers operate by allocating *T* shares among all applications. An application can receive *N* shares of time, thus ensuring that the application will have N/T of the total processor time. 

As an example, assume that a total of T = 100 shares is to be divided among three processes, A, B, and C. A is assigned 50 shares, B is assigned 15 shares, and C is assigned 20 shares. This scheme ensures that A will have 50 percent of total processor time, B will have 15 percent, and C will have 20 percent.

Proportional share schedulers must work in conjunction with an admission-control policy to guarantee that an application receives its allocated shares of time. 

## Algorithm evaluation

This section covers the various methods which may be used to determine the appropriate scheduling algorithms to use. 

### Deterministic modeling

System by which a workload is defined and simulated in each algorithm to directly compare the performance. 

### Probabilistic Modeling (Queueing-Network Analysis)

When CPU scheduling processes and their characteristics vary unpredictably, deterministic modeling (fixed set of processes/times) is insufficient. We turn to probabilistic methods and simulations.

* **Concept:** Models the computer system as a network of "servers" (CPU, I/O devices), each with a queue.
* **Key Distributions:**
    * **CPU/I/O Burst Distribution:** Describes the probability of a process's CPU or I/O burst length (often exponential, defined by its mean).
    * **Arrival-Time Distribution:** Describes when new processes enter the system.
* **Queueing Analysis Parameters:**
    * `n`: Average long-term queue length.
    * `W`: Average waiting time in the queue.
    * `λ`: Average arrival rate of new processes.
* **Little's Formula:** `n = λ × W` allows calculating any one parameter if the other two are known.
* **Limitations:**
    * Mathematical complexity limits algorithms and distributions that can be handled.
    * Often uses mathematically "tractable" but unrealistic distributions, leading to approximations of real systems.

## Simulations

* **Concept:** Creates a model of the system to mimic its behavior over time.
* **Method:** Uses random-number generators to simulate events (processes, bursts, arrivals, departures) based on probability distributions.
    * **Mathematical Distributions:** Uniform, exponential, Poisson.
    * **Empirical Distributions:** Derived from measurements of actual system behavior.
* **Trace Files:** For higher accuracy, monitor a real system and record actual event sequences. This "trace" then drives the simulation.
* **Advantage:** More accurate evaluation than pure queueing models, especially for complex algorithms/distributions.

## Algorithm Flexibility & Tuning

* **Optimal Algorithms:** Most flexible scheduling algorithms allow system managers/users to tune them for specific applications.
* **API Modification:** APIs (e.g., Java, POSIX, Windows) allow modifying process/thread priorities.
* **Downfall of Tuning:** Performance tuning for specific situations often doesn't generalize to improved performance in broader scenarios.

