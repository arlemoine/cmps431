---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: Intro to Operating Systems
topicDate: 2025-06-09
---
# Intro to Operating Systems
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Module objectives
- Discuss what an operating systems is and how operating systems are used in various computing environments.
- Identify services provided by an operating system.
- Explain how system calls are used to provide operating system services.
- Compare and contrast monolithic, layered, microkernel, modular, and hybrid strategies for designing operating systems.

## Operating System (OS)

That one program running at all times on the computer - usually called the **kernel**. May also include system programs and support programs which include drivers and loadable kernel modules. 

Mobile OSs include middleware, a set of software frameworks that provide additional services to application developers. 

Large portion of OS code dedicated to IO management.

## 2 General Schemes for IO Control

### Polling

CPU periodically checks status of a peripheral and acts accordingly. Works well for data acquisition on a set periodic schedule but wastes CPU time.

### Interrupts
 
 CPU has dedicated interrupt signal which is triggered by a peripheral when it needs servicing. Suitable for event-driven comms and does not waste cycles. Suitable when CPU must respond to peripheral events which are not on a set schedule. Utilizes an interrupt encoder on the bus to send the signal to the CPU. Interrupt encoder is a hardware device. 

### How Interrupt I/O Works

- CPU receives interrupt signal indicating a service request
- CPU suspends what it is doing (saves its program counter)
- CPU jumps to a general service routine
	- Disables further interrupts
	- Identifies which peripheral is to be serviced via a signal (interrupt vector) produced by the interrupt encoder.
	- Jumps to service routine for that peripheral
- Peripheral's service routine performs the service
- Upon return of the service routine, interrupts are re-enabled and the CPU returns to what it was doing via the program counterl.

### Maskable vs unmaskable Interruptable Inputs

- CPU interrupt inputs may be maskable, meaning that critical programs may "mask off" to avoid interruption of critical processes. Think low priority.
- They may be unmaskable, ensuring that critical events are detected properly. Think high priority.

## Direct Memory Access (DMA) 

Allows transferring of data between peripheral and main memory. Coordination of transfer is outsourced to dedicated transfer controller without intervention by CPU. 

Some information forwarded to the DMA controller from the CPU: memory start address, block size, peripheral ID. 

DMA controller is a simpler design than the CPU but can perform the required task - freeing up the CPU to focus on other things. DMA controller sends the control signals to the devices involved. 
	
## Direct Data Transfer (DDT)

Transfers between 2 peripherals. 
	Instead of a bus, uses a **non-blocking switch** (networking switch). Allows multiple components to talk to each other concurrently. 

## Storage structure

First program to run on computer startup is the **bootstrap program** which loads the OS.
Its primary job is to perform initial system checks (like making sure the hardware is working) and then, crucially, to locate and load the more complex operating system (OS) into RAM.

Since RAM is volatile, we don't rely solely on RAM for boot code. Rather, boot code is stored in  non-volatile **electrically erasable programmable read-only memory** (EEPROM) and other forms of on-volatile **firmware**. 

Main computer memory stores data via non-volatile hard disks or semiconductor based cards (SSDs). We'll refer to **non-volatile storage** as NVS. 

Storage capacity:
![[memory_storage_capacities.png]]

## Blade servers

Made up of multiple processor boards, I/O boards, and networking boards all in the same chassis running their own OS. 

## Clustering

To provide high-availability service that will continue even if one or more systems in the cluster fail. Creates redundancy. 
Ability to continue providing service among the surviving hardware is called **graceful degradation**. If something fails again, you might notice a change in performance.
Some systems are more sturdy than graceful degadation and are called **fault tolerant** where they can suffer a failure of a single component and still operate the same. 
**Assymetric clustering**: one machine in hot-standby mode while the other runs applications. The hot-standby host machine does nothing but monitor the active server and takes control if needed. 
**Symmetric clustering**: two or more hosts are running applications and monitoring each other.

## Parallelization

Divides program into separate components running in parallel on individual processors in a cluster. 

A **distributed lock manager** (DLM) used to help provide locking of resources to implement a two-phase locking protocol to prevent contention among processors sharing resources. 

**Storage-area networks** (SANs): allow many systems to attach to a pool of storage. Assists in redundancy by allowing a processor take-over to happen without loss of data. 

## Virtual memory

Allows execution of a process that is not completely in memory. 
Main advantage: enables us to run programs that are larger than actual physical memory. 

## Dual-mode and multimode operation

Ensure degree of separation between OS code and user-defined code to prevent system corruption. 
These are typically called user mode and kernel mode (or supervisor/system/privileged mode). 
The mode bit is a bit added during operation to indicate the current mode as kernel (0) or user (1) to distinguish between OS tasks and user tasks. 
When a trap or interrupt occurs, the hardware switches from user mode to kernel mode (mode bit changes to 0). 
A degree of protection is provided by some machine instructions being designated as priviliged instructions (which only run in kernel mode). 
CPUs designed around virtualization have a separate mode to indicate when the **virtual machine manager** (VMM) is in control of the system.

## System calls

Provide means for user program to ask the OS to perform tasks reserved for the OS on the user's behalf. When system call is executed, it is typically treated as a **software interrupt** by the hardware. Then, control passes to a service routine under OS control and the mode bit is set to kernel mode. 
These system calls are the only gateway between user space and kernel space. 

## Resource management

An OS is a **resource manager** responsible for:
- Process management 
- Memory management 
- File-system management 
- Mass-storage management 
- Cache management 
- I/O system management 

## Virtualization

A technology which allows us to abstract hardware of a cincle computer (CPU, memory, disk drives, network interface cards, etc) into several different execution environments.
Creates illusion that each separate environment is running on its own computer. 
Allows OS to run as applications within other OSs. 

## Kernel data structures

Linked lists, stacks, or queues
**Singly linked list**: Each item points to its successor. 
**Doubly linked list**: Given item can point to both its predecessor and its successor. 
**Circularly linked list**: Last element in the list points to the first element rather than NULL.
**Stack**: Sequentially ordered data structure utilizing principle of "last in, first out". An OS often uses stack when invoking function calls. Parameters, local variables, and return address pushed onto stack when function s called and then they are popped off of the stack as executed. 
**Queue**: Sequentially ordered data structure using "first in, first out" principle. Items removed from a queue in the order in which they were inserted. 
**Tree**: Data structure used to represent data hierarchically. Data values in tree structure are linked through parent-child relationships.
**Hash function**:  Takes data as input, performs numeric operation on the data, and returns numeric value. Then, this value can be used as an index within in array to find a well-organized and spaced out location for the related data. If hash collisions arise (same location for two separate pieces of data), a linked list can be used at this location which contains all items with the same hash value. 

## Bitmaps

A binary string of n digits that can be used to represent the status of n items. This might mean that 1 represents "available" while 0 represents "unavailable" or vice-versa. 
Example bitmap: 0 0 1 0 1 1 1 0 1

## Computing environments

Traditional computing
Mobile computing
Client-server computing
Peer-to-peer computing: All nodes in system are considered peers and each may act as either a client or server depending on the need. 
Cloud computing
- Software as a service (SaaS), such as MS Office on cloud
- Platform as a service (PaaS), such as MS Azure or AWS
- Infrastructure as a service (IaaS), like PaaS but already with applications software. 
Real-time embedded systems, such as a home security system or Google Home speaker. 

## Study Guide

What operating systems do  
	Manages computer's hardware
	Provides basis for programs
	Acts as intermediary between user and hardware
	![[Pasted image 20250610154609.png]]
	
Computer-system organization  
	Hardware, software, and data
Computer-system architecture  
	
Operating-system Structure  
Operating-system operations  
Process management  
Memory management  
Storage management  
Security and protection  
Distributed systems  
Computing environments  
Free and open-source operating systems  
            
## Additional

An operating system acts as an intermediary between the user of a computer and the computer hardware; it is the manager of processes running on the computer system and it allocates hardware resources (memory, CPU, etc.) to the application processes. The purpose of an operating system is to provide an environment in which a user can execute programs in a convenient and efficient manner. It is a software which manages all of the other application programs in a computer. The application programs use the service of the operating system by making requests for services through specific application program interfaces.

An operating system is software that manages the computer hardware. The hardware must provide appropriate mechanisms to ensure the correct operation of the computer system and to prevent programs from interfering with the proper operation of the system.

Internally, operating systems vary greatly in their makeup, since they are organized along many different lines. The design of a new operating system is a major task, and it is important that the goals of the system be well defined before the design begins.

Because an operating system is large and complex, it must be created piece by piece. Each of these pieces should be a well-delineated portion of the system, with carefully defined inputs, outputs, and functions.

This module consists of two chapters:
1. A general introduction to operating systems, and
2. Operating system structures.

