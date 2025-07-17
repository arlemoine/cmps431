---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: Processes
topicDate: 2025-06-17
---
# Processes
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Study Topics

3. Processes
   3.1 Process concept  
   3.2 Process scheduling  
   3.3 Operations on processes  
   3.4 Interprocess communication  
   3.5 Examples of IPC systems  
   3.6 Communication in client-server systems  
   3.7 Summary  
         Practice exercises

## Intro

**Process**: A program in execution. May need: 
- CPU time
- Memory
- Files
- I/O devices

Need to know:
- Various components of a process
- How processes are created/terminated in an OS
- How interprocess communication is accomplished via shared memory and message passing
- Related concepts of pipes and POSIX shared memory
- Client-server communication via sockets and remote procedure calls

## Program execution flow

The PC content is saved on a stack of registers (link registers) or other memory. The following is the general flow: 

1. Content of **program counter** (PC) is used to fetch the next instruction to be executed. 
2. Content of program counter is auto-incremented to point to the next instruction after the current one executes. 
3. Current instruction is executed.

### Program sequence jump

If a program sequence must jump, the PC contents (pointer to "next instruction") are saved for later and the content of the PC is overwritten with the address of the "jump-to" instruction. 

On encountering a return instruction, the saved PC content is restored to the PC to resume. 

## Process status and state

### Process status

Process **status** is represented by the value of the PC and the contents of the processor's registers (**system state**). 

### Memory layout

- Text section: Executable code
- Data section: Global variables
- Heap section: Memory dynamically allocated during program run-time
- Stack section: Temporary data storage when invoking functions

### Process state

The **state** of a process is defined in part by the current activity of the process. Potential states include:
- New: Process is being created
- Running: Instructions are being executed
- Waiting: Waiting for an event to occur
- Ready: Process waiting to be assigned to processor
- Terminated: Process has finished execution

![[process_state.png]]

## Process control blocks

Each process is represented in the OS by a **process control block** (PCB). It serves as the repository for all the data needed to start/stop a process:
- Process state: New, ready, running, waiting, halted, etc.
- Program counter: Indicates address of next instruction. 
- CPU registers to be used
- CPU scheduling info
- Memory management info
- Accounting info: Amount of CPU and time used, time limits, account numbers, job/process numbers, etc. 
- I/O status: List of I/O devices allocated to process, list of open files, etc. 

## Threads

Modern OSs allow a process to have multiple threads of execution and therefore perform more than one task at a time. 

## Process scheduling

Processes entering the system are put on a **ready queue** where they will wait to execute on a CPU core. The queue is implemented as a **linked list**. A ready-queue header points to the first PCB in the list, and each PCB includes a pointer field which points to the next PCB in the ready queue. 

If a process is executing and comes across a wait event, the process is placed in a wait queue. 

### Queueing diagram

![[queueing_diagram.png]]

## CPU scheduling

A **CPU scheduler** selects a process in the ready queue and allocates a CPU core to it. If a process requires a CPU core for long durations, the CPU scheduler forcibly removes the CPU core to allot it to another process. The stopped process may cycle back into the ready queue. 

Sometimes a process may be removed from memory and then reintroduced into memory later where it left off. This is called **swapping** because a process is "swapped out" from memory to disk and later back to memory. This is typically only necessary when memory has been overcommitted and must be freed up.

When an interrupt occurs, the system needs to save the current **context** of the running process, suspending the process to later resume. The context is represented in the PCB of the process. It includes:
- Values of the CPU registers at time of suspension
- Process state
- Memory management info

## Process operations

Many OSs identify processes by a unique **process identifier** (pid). When a process creates a new process, the parent may either execute concurrently with the child process or wait for it to finish. 

The **fork()** system call creates a new process which consists of a copy of the address space of the original process. Both processes continue execution with one difference: the return code for the fork() is ero for the child process and process identifier of the child is returned to the parent. 

After a fork(), one of the two processes typically uses the **exec()** system call to replace the process's main memory space with a new one. 

On Windows, processes are created in the Windows API via **CreateProcess()**. 

### Process termination

Termination happens when execution is done and the process asks the OS to delete it by using the **exit()** system call. All the resources of the process are deallocated and reclaimed by the OS. After being terminated, its entry remains in the process table until the parent process calls wait() because the process table contains the child process's exit status. A process that has terminated but whose parent has not yet called wait() is known as a **zombie** process. 

Generally, only a parent can terminate its child process. If a process terminates, then typically, all its children must also be terminated. This is called **cascading termination**. 

### Mobile OS process termination order

- Empty processes
- Background processes
- Service processes
- Visible processes
- Foreground processes

## Interprocess communication

There are 2 fundamental models of **interprocess communication** (IPC): **shared memory** and **message passing**. 

### Shared memory model

A region of memory shared for cooperating processes. Can be faster than message passing due to less system calls and the required kernel intervention. 

Processes wishing to communicate must establish a shared memory segment and attach it to their address space.

With shared memory, communicating processes need some synchronization. A process of producing data must place data in some **buffer** (in shared memory) before a consumer process can use the data.

The **unbounded buffer** places no practical limit on the size of the buffer. The consumer may have to wait for new items, but the producer can always produce new items. 

The **bounded buffer** assumes a fixed buffer size. In this case, the consumer must wait if the buffer is empty, and the producer must wait if the buffer is full.
### Message passing model

Communication is via messages exchanged between processes. Useful for smaller amounts of data. Also easier to implement in a distributed system. 

Direct communication allows sender and receiver to call on each other directly.
- Link between every process that communicates. They only need to know each other's identity. 
- Link is between exactly two processes. 

With indirect communication, messages are sent to and received from mailboxes or **ports**. 
- Link between pair of processes only if they share the same mailbox/port. 
- Link may be between more than two processes. 
- A number of different links may exist for one process. 

To synchronize exchanges, the send() and receive() signals of a process may be either a **blocking send/receive** or an **unblocking send/receive**. 
- **A blocking send** causes the sending process to **suspend its execution** until the message has been successfully sent (e.g., delivered to the receiver or placed in a buffer).
- **A blocking receive** causes the receiving process to **suspend its execution** until a message is available and successfully received.
- **A non-blocking send** allows the sending process to **immediately continue its execution** after initiating the send, without waiting for the message to be delivered or acknowledged.
- **A non-blocking receive** attempts to receive a message and, if none is available, **immediately returns** (often with an indicator that no message was found) without suspending the process.

Messages, regardless of the model, are passed through a queue. There are 3 possible implementations of the queue:
- **Zero capacity**: Queue has max length of zero and the sender must block until recipient receives the message. 
- **Bounded capacity**: Queue has finite length n, allowing for n messages in the queue. If the queue was full,  sender must block until space is available.  
- **Unbounded capacity**: Queues length is potential infinite and sender never blocks. 

## Pipes

Acts as conduit for 2 processes to communicate. Two types typically used: 
- **Ordinary pipes**: Only exists when needed. Consists of a read end (sender) and read end (receiver).  
- **Named pipes**: Bidirectional (but still utilizes half duplex transmission). Several process can use it instead of just two. Continue to exist after use until explicitly deleted. Referred to as a FIFO in UNIX systems. 

## Sockets in client-server systems

A **socket** is an endpoint for communication. A pair of processes communicating over a network employs a pair of sockets - one for each process. A socket is defined by **an IP address concatenated with a port number** (e.g. 146.86.5.20:22). 

In general, the server waits for incoming client requests by listening to a specified port. Once a request is received, the server accepts a connection from the client socket to complete the connection. 

Common services such as SSH, FTP, and HTTP listen to **well-known** ports, defined as any port below 1024, used to implement standard services. 

The pair of sockets used for communication consist of the process connected to the socket of one machine communicating with the process of another machine through the second machine's socket. So the channel for communication is process 1 (machine 1) -> socket 1 -> socket 2 -> process 2 (machine 2). 