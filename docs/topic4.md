---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: Operating System Structures
topicDate: 2025-06-12
---
# Operating System Structures
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## Overview

We can view an operating system from several vantage points. 

One view focuses on the **services** that the system provides; another, on the **interface** that it makes available to users and programmers; a third, on its **components** and their interconnections. 

In this chapter, we explore all three aspects of operating systems, showing the viewpoints of users, programmers, and operating system designers. We consider what services an operating system provides, how they are provided, how they are debugged, and what the various methodologies are for designing such systems. Finally, we describe how operating systems are created and how a computer starts its operating system.

## Objectives

- Identify services provided by an operating system.
- Explain how system calls are used to provide operating system services.
- Compare and contrast monolithic, layered, microkernel, modular, and hybrid strategies for designing operating systems.

## Types of system services

- Program execution
- I/O operations
- File systems
- Communication
- Resource allocation
- Accounting
- Error detection
- Security

## Application programming interface (API)

Developers design programs often according to an **application programming interface** (API) which specifies a set of functions and passable parameters available to a programmer. 

Three of the **most common APIs** available to developers: 
1. Windows API for Windows systems
2. POSIX API for POSIX-based systems (virtually all of UNIX, Linux, and MacOS)
3. Java API for programs running on the Java VM

### Benefits of APIs

- Portability: Can expect a program to compile and run on any system supporting the API.
- System calls can be difficult to work with compared to the plug-and-play API
- The caller doesn't need to know anything about how the API backend works

## Run-time environment (RTE)

The run-time environment (RTE) provides a system-call interface to serve as the link to system calls made available by the OS. The RTE is the full suite of software needed to execute aplications in a given programming language, including its compilers or interpreters as well as other software, such as libraries and loaders.

## System Services

3 general methods to pass parameters to OS for invocation of system services:
1. Pass parameters in registers
2. Store parameters in memory and pass the address of the memory block to a register
3. Push parameters onto a stack by the program and pop off the stack by the OS

### Types of system calls

- **Process control**: Create, terminate, load, execute, get, set, wait, signal, allocate, etc. 
- **File management**: Create, delete, open, close, read, write, reposition, get attributes, set attributes. 
- **Device management**: Request, release, read, write, reposition, get attributes, set attributes, attach, detach. 
- **Information maintenance**: Get time, set time, get data, set data, get attributes, set attributes. 
- **Communications**: Create, delete, connect, send, receive, transfer, attach, detach.
- **Protection**: Get permission, set permission. 

### Linkers and Loaders

After a compiler translates source code into one or more machine language file(s) called object file(s), an object linker performs 3 functions:
1. Combine object files into single executable program
2. Links **library files** (collections of precompiled code that has been packaged for reuse in other programs)
3. Makes sure cross-file dependencies are resolved properly. 

The loader then prepares a prcompiled program for execution by loading it into memory, adjusting addresses, and starting the program. 

## OS Structure

- Monolithic (traditional)
- Layered
- Microkernels
- Modules
- Hybrid

## OS Booting

Boot process:
1. The bootstrap program, or boot loader, a small piece of code, locates the kernel. 
2. The kernel is loaded into memory and started. 
3. The kernel initializes hardware. 
4. The root file system is mounted. 

Some computer systems use a multistage boot process. With this, a small boot loader in the nonvolatile firmware known as **BIOS** is run. This initial boot loader usually only loads a second boot loader located at a fixed disk location called the **boot block**. The program stored in the boot block might load the entire OS into memory and begin execution. 

Many modern computer systems replaced the BIOS-based boot process with **UEFI** (Unified Extensible Firmware Interface) which has better support for 64-bit systems and larger disks. UEFI is a single, complete boot manager and therefore is faster than the multistage BIOS boot process. 

**GRUB** is an open-source bootstrap program for Linux and UNIX systems. Boot parameters for the system are set in a GRUB configuration file, which is loaded at startup. GRUB is flexible and allows changes to be made at boot time, including modifying kernel parameters and even selecting among different kernels that can be booted. 

## System Failures

Most OSs write error info to a **log file** to be probed for review later. When a process fails, the OS takes a **core dump**, capturing the memory of the process. A failure in the kernel is known as a crash, and when a crash occurs, error information is saved to a log file and the memory state is saved to a **crash dump**. 

## Study guide

2. Operating-System Structures
   2.1 Operating-system services  
   2.2 User and operating-system interface  
   2.3 System calls  
   2.4 Types of System calls  
   2.5 System programs  
   2.6 Operating-system design and implementation  
   2.7 Operating-system structure  
   2.8 Virtual Machines  
   2.11 System Boot  
   2.12 Summary  
   2.13 Practice exercises

## Additional

- System calls provide interface to the services made available by an operating system. System calls are the only gateway between user space and kernel space. 