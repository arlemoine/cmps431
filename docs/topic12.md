# Parallel Computing
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

# Operating Systems & Parallel Computing Study Guide

## I. File Systems Overview

**Definition:** The file system provides the mechanism for on-line storage of and access to both data and programs of the operating system and all the users of the computer system. It consists of a collection of files, each storing related data, and a directory structure, which organizes and provides information about all the files in the system.

**Module Structure (3 Parts):**
1.  **Files & Directory Structures:** In the first part, we consider the various aspects of files and the major directory structures and we discuss the sharing of files among multiple processes, users, and computers, and how they are protected when multiple users may access the file system.
2.  **File Use, Storage & Recovery:** In the second part, we explore ways to structure file use, to allocate storage space, to recover freed space, to track the locations of data, and to interface other parts of the operating system to secondary storage.
3.  **File System Implementation & Remote File Systems:** In the third part, we discuss the details of file systems and their implementation, explore booting and file sharing, and describe remote file systems using NFS as an example.

**Module Objectives:**
* Describe the function and interfaces of file systems and the implementation of local file systems and directory structures.
* Articulate file-system design tradeoffs, including access methods, file sharing, file locking, and directory structures.
* Explain booting, file sharing, file-system protection, and recovery from file system failures.
* Describe remote file systems, using NFS as an example.

## II. Parallel Computing Primer

**Definition:** Parallel Computing is ubiquitous to modern computing. Whether people realize it or not, we are all using one form of parallel computing or another nearly any time we use a computer.

### A. Instruction-Level Parallelism (ILP) vs. Task-Level Parallelism (TLP)

* **Instruction-Level Parallelism (ILP):**
    * **Focus:** Maximizing the number of instructions executed concurrently within a single processor core.
    * **Unit of Parallelism:** Individual instructions.
    * **Scope:** Within a single thread.
    * **Handled by:** Hardware (CPU architecture). It is managed by the CPU hardware, not by explicit code.
    * **Granularity:** Fine.
    * **Techniques:** Pipelining, superscalar execution, and out-of-order execution to overlap instruction execution phases and utilize available resources more efficiently.
    * **Key Point:** ILP is an inherent property of the code, depending on how many instructions are independent. CPU architectures try to detect and use available ILP.

* **Task-Level Parallelism (TLP):**
    * **Focus:** Executing multiple independent tasks or processes at the same time.
    * **Unit of Parallelism:** Entire tasks or threads.
    * **Scope:** Across threads or processes.
    * **Handled by:** Software and OS/threading.
    * **Granularity:** Coarse.
    * **Key Point:** This strategy focuses on exploiting the parallelism inherent in the problem itself, rather than within individual instructions.

| Feature                   | Instruction-Level Parallelism    | Task-Level Parallelism       |
| :------------------------ | :------------------------------- | :--------------------------- |
| Unit of parallelism       | Individual instructions          | Entire tasks or threads      |
| Scope                     | Within a single thread           | Across threads or processes  |
| Handled by                | Hardware (CPU architecture)      | Software and OS/threading    |
| Granularity               | Fine                             | Coarse                       |

### B. Pipelining

* **Concept:** Increases the performance of the system with simple design changes in the hardware. It facilitates parallelism in execution at the hardware level. Multiple instructions are executed in a timing state, in an overlapped manner.
* **Purpose:** Does not reduce the execution time of individual instructions but reduces the overall execution time required for a program.
* **Stages (Typical 4-stage):**
    1.  **IF** - Instruction fetch
    2.  **ID** - Instruction decode
    3.  **IE** - Instruction execute
    4.  **RW** - Result writing
* **Stages (Typical 5-stage):**
    1.  **IF** - Instruction Fetch
    2.  **ID** - Instruction Decode
    3.  **OF** - Operand Fetch
    4.  **EX** - Instruction Execution
    5.  **WB** - Write Back
* **Latency:** A measure of how long an instruction takes to complete from start to finish. For a 5-stage pipeline, instruction latency is 5 cycles.
* **Throughput:** One instruction is finished on every single cycle after time 't4' (in a 5-stage pipeline timeline). Processor bandwidth is one instruction per cycle.
* **Performance Calculation (ICT - Instructions Completed per unit Time):** $ICT = \frac{\text{clock cycles}}{\text{number of instructions completed}}$
    * Example (5 instructions, 4-stage pipeline):
        * With pipelining: 8 cycles for 5 instructions, $ICT = 8/5 = 1.6$.
        * Without pipelining: 20 cycles for 5 instructions, $ICT = 20/4 = 4$.
* **Realities/Problems:**
    * All stages must execute in the same amount of time, so "cycle" time must be as long as the stage that takes the maximal amount of time.
    * Care must be put into decomposing the design into stages.
    * Idealistic calculations assume pipeline to always be "full".

### C. Pipeline Hazards

* **Definition:** A pipeline works well when all stages work in synch and complete their tasks in synch. If one cannot complete on time problems arise.
* **Types:**
    * **Data Hazards:** Occur when an instruction needs data that is not yet available. This is a data dependency.
    * **Structural Hazards:** When the same hardware is needed by more than one instruction in the pipeline.
    * **Control Hazards (Branch Hazards):** When changes to the program counter affect the pipeline execution.
* **Note:** The interrupt stage has been left out of all examples. The modern CPU cycle includes the interrupt stage at the end (fetch, decode, execute, interrupt).

### D. Superscalar Execution

* **Concept:** Refers to a hardware implementation that can issue and execute multiple instructions per clock cycle.
* **Mechanism:** The processor uses multiple execution units and issue logic to take advantage of ILP in the instruction stream.
* **Relationship to Pipelining:** Superscalar processors often incorporate pipelining as well. A pipelined processor can be made superscalar by having multiple execution units within each pipeline stage. They are complementary techniques that can be combined to achieve significant performance improvements.
* **Key Differences (Superscalar vs. Pipelining):**
    * **Focus:** Pipelining focuses on breaking down the execution process into stages, while superscalar execution focuses on utilizing multiple execution units.
    * **Mechanism:** Pipelining overlaps instructions in a single execution unit, while superscalar execution uses multiple execution units to process multiple instructions simultaneously.
* **Advantages (Superscalar):**
    * Higher throughput: It can potentially execute more instructions per cycle than pipelining alone.
    * Improved performance: Can lead to significant performance gains, especially for applications with inherent parallelism.
    * Flexibility: Can be designed to execute different types of instructions simultaneously.

### E. Parallelism Limits

* **Data Dependencies:** Not everything can be parallelized. For instance, the subtraction $n3$ cannot be done without first knowing the values $n1$ and $n2$.
* **Overhead:** Parallelization introduces extra code for synchronization and communication (fork and join sections). The cost of constructing and managing a thread can be greater than the computation time of the subtask itself. If the subtask is too small, the parallel algorithm can actually be slower than the serial algorithm.
* **Conclusion:** Even for tasks that can be parallelized, the programmer has to evaluate the pros and cons and implement the optimal degree of parallelization. More parallel does not always imply faster!

### F. Task Parallelism Examples

* **Python (using `multiprocessing`):**
    * **Serial Version:** Executes tasks one after another.
        ```python
        import time

        def task(name):
            print(f"Starting {name}")
            time.sleep(2)
            print(f"Finished {name}")

        def main():
            start = time.time()
            task("Task 1")
            task("Task 2")
            task("Task 3")
            end = time.time()
            print(f"Total time (serial): {end - start:.2f} seconds")

        if __name__ == "__main__":
            main()
        # Output: ~6.00 seconds
        ```
    * **Parallel Version:** Tasks run concurrently using separate processes.
        ```python
        from multiprocessing import Process
        import time

        def task(name):
            print(f"Starting {name}")
            time.sleep(2)
            print(f"Finished {name}")

        def main():
            start = time.time()
            p1 = Process(target=task, args=("Task 1",))
            p2 = Process(target=task, args=("Task 2",))
            p3 = Process(target=task, args=("Task 3",))
            p1.start()
            p2.start()
            p3.start()
            p1.join()
            p2.join()
            p3.join()
            end = time.time()
            print(f"Total time (parallel): {end - start:.2f} seconds")

        if __name__ == "__main__":
            main()
        # Output: ~2.00 seconds
        ```
* **C (using `fork()`):**
    * **Serial Version:**
        ```c
        #include <stdio.h>
        #include <unistd.h>
        #include <time.h>

        void task(const char* name) {
            printf("Starting %s\n", name);
            sleep(2);
            printf("Finished %s\n", name);
        }

        int main() {
            time_t start = time(NULL);
            task("Task 1");
            task("Task 2");
            task("Task 3");
            time_t end = time(NULL);
            printf("Total time (serial): %ld seconds\n", end - start);
            return 0;
        }
        # Output: 6 seconds
        ```
    * **Parallel Version:**
        ```c
        #include <stdio.h>
        #include <unistd.h>
        #include <sys/wait.h>
        #include <time.h>

        void task(const char* name) {
            printf("Starting %s\n", name);
            sleep(2);
            printf("Finished %s\n", name);
        }

        int main() {
            time_t start = time(NULL);

            pid_t p1 = fork();
            if (p1 == 0) {
                task("Task 1");
                return 0; // child exits
            }

            pid_t p2 = fork();
            if (p2 == 0) {
                task("Task 2");
                return 0;
            }

            pid_t p3 = fork();
            if (p3 == 0) {
                task("Task 3");
                return 0;
            }

            // Parent waits for all children
            wait(NULL);
            wait(NULL);
            wait(NULL);

            time_t end = time(NULL);
            printf("Total time (parallel): %ld seconds\n", end - start);
            return 0;
        }
        # Output: 2 seconds
        ```

### G. Bubble Sort Optimization Example (Task Parallelism Applied)

* **Problem:** Basic Bubble Sort is of time complexity order $O(n^2)$. Sorting a data set of 1000 numbers would take about 1 million comparisons.
* **Optimization Strategy:** Divide the data set into parts, run bubble sort on each part separately, then merge the sorted parts.
* **Comparison Count Improvement:**
    * Original (1000 numbers): $1000^2 = 1,000,000$ comparisons.
    * Divided into 5 pieces (200 numbers each): $5 \times 200^2 = 5 \times 40,000 = 200,000$ comparisons for the bubble phase.
    * Total with merge: Between 188,000 and 189,000 compares. This is a significant improvement in efficiency.
* **CPU Time Improvement (Python example):**
    * Non-optimized: `cpu time used = 0.109375`
    * Optimized (sequential split/merge): `total time needed for bubble and merge = 0.03125`
    * This is a speed up by a factor of 3.5 times.
* **Python Parallelization Advice:**
    * **CPU-bound tasks:** If tasks are computationally intensive, use `multiprocessing` or `concurrent.futures.ProcessPoolExecutor` to leverage multiple CPU cores.
    * **I/O-bound tasks:** If tasks involve waiting for I/O operations, use `asyncio` to achieve concurrency without blocking the main thread.
