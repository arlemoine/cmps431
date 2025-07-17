---
tags:
  - SELU
courseNumber: CMPS 431
year: "2025"
courseTopic: File System
topicDate: 2025-07-17
---
# File System
___

>[!Links]
>1. [[cmps_431|This Course]]
>2. [[2025_summer| This Semester]]
>3. [[00_Southeastern|Southeastern Main]]

## File system management

The file system provides mechanisms for online storage and access to data and programs. It consists of a collection of files and a directory structure for organization.

### Key aspects

* **Files:** Collections of related data.
    * Attributes: Name, identifier, type, location, size, protection, timestamps and user identification. Some newer file systems support extended attributes like character encoding and file checksum.

* **Directory structure:** Organizes and provides information about files.
    * Information about all files is kept in the directory structure, residing on the same device as the files.
    * A directory entry typically consists of the file's name and its unique identifier, which locates other file attributes.
    * Can be viewed as a symbol table translating file names into file control blocks.
    * Operations: Search, create, delete, list, rename, traverse.

### File operations

A file is an abstract data type with defined operations. The operating system provides system calls for these.

* **Creating a file:**
    1. Find space in the file system.
    2. Make an entry for the new file in a directory.
    * A **file-control block (FCB)** is allocated, containing file information (ownership, permissions, access dates, block locations). The directory is updated with the new file name and FCB.

* **Opening a file:** All file operations except create and delete require an `open()` call first, which binds a handler to the file object. If successful, it returns a file handle used in other calls.

* **Writing a file:**
    * System call specifies open file handle and information to be written.
    * System searches directory for file's location.
    * System maintains a write pointer for sequential writes, updated after each write.

* **Reading a file:**
    * System call specifies file handle and memory location for the next block.
    * Directory searched for entry.
    * System maintains a read pointer for sequential reads, updated after each read.
    * A per-process **current-file-position pointer** is used for both read and write operations for efficiency.

* **Repositioning within a file (file seek):** The current-file-position pointer of the open file is moved to a given value. Does not involve I/O.

* **Deleting a file:**
    * Search directory for the named file.
    * Release all file space for reuse.
    * Erase or mark as free the directory entry.
    * If hard links (multiple names for the same file) exist, the actual file contents are not deleted until the last link is removed.

* **Truncating a file:** Erases file contents but keeps attributes. Resets file length to zero and releases its space.

* **Open-file table:**
    * The operating system maintains a system-wide table with information about all open files.
    * File operations use an index into this table, avoiding search.
    * When a file is closed, its entry is removed.
    * Also, per-process tables point to the system-wide table.
    * System-wide table contains process-independent info (location on disk, access dates, size).
    * An **open count** indicates how many processes have the file open.

### File locks

* Facilities for locking an open file (or sections) to prevent other processes from gaining access.
* Useful for shared files (e.g., system log files).
* Similar to reader-writer locks:
    * **Shared lock:** Multiple processes can acquire concurrently (like a reader lock).
    * **Exclusive lock:** Only one process at a time can acquire (like a writer lock).
* Not all OS provide both types; some only exclusive.

## Access methods

### Sequential access

* Information in the file is processed in order, one record after the other.

### Direct access

* File is made of fixed-length logical records.
* Programs can read and write records rapidly in no particular order.
* File viewed as a numbered sequence of blocks or records.

## Directory structure types

* **Single-level directory:**
    * All files in the same directory.
    * Simple to support and understand.
    * Limitations: All files must have unique names; difficult with many files or multiple users.

* **Two-level directory:**
    * Each user has their own **user file directory (UFD)**.
    * UFDs list only files of a single user.
    * System's **master file directory (MFD)** is indexed by user name/account number, pointing to the user's UFD.

* **Tree-structured directories:**
    * Natural generalization to a tree of arbitrary height.
    * Users can create subdirectories to organize files.
    * Most common directory structure.
    * Has a root directory; every file has a unique path name.

* **Acyclic-graph directories:**
    * Allows directories to share subdirectories and files (no cycles).
    * Same file/subdirectory can be in two different directories.
    * Implemented using **symbolic links** (e.g., in UNIX).
    * **Deletion in acyclic graphs:**
        * A reference count of 0 means no more references, and the file can be deleted.
        * If cycles exist, a reference count may not be 0 even if the file is unreachable. In this case, **garbage collection** is needed:
            1. Traverse file system, marking accessible items.
            2. Second pass collects unmarked items to a free space list.

## Protection

Protection mechanisms provide controlled access by limiting the types of file access.

### Types of operations controlled

* **Read:** Read from the file.
* **Write:** Write or rewrite the file.
* **Execute:** Load the file into memory and execute it.
* **Append:** Write new information at the end of the file.
* **Delete:** Delete the file and free its space.
* **List:** List the name and attributes of the file.
* **Attribute change:** Changing the attributes of the file.

### Access control methods

* **Identity-dependent access:** Access depends on user identity.
* **Access-control list (ACL):** Associates with each file/directory a list of user names and allowed access types for each.
* **Owner, group, and other scheme (common approach):**
    * **Owner:** User who created the file; has most control.
    * **Group:** Set of users who are sharing the file with similar access needs. Owner is not automatically a group member.
    * **Other:** All other users in the system.
    * Owner and group IDs are stored with file attributes. When an operation is requested, user ID and group IDs are compared to determine applicable permissions.
    * Example: Sara (owner) can do all operations on `book.tex`. Jim, Dawn, Jill (in 'text' group) can read/write but not delete. All others can read.
    * Modern systems like Solaris combine this with ACLs for finer-grained control.

## File sharing

To implement sharing and protection, systems maintain more file and directory attributes than single-user systems.

* System maintains file/directory owner and group concepts.
* Owner can change attributes and grant access.
* Group defines a subset of users who can share access.
* Permissions for group members and others are definable by the file's owner.

## Directory implementation - allocation methods

### Contiguous allocation

* Each file occupies a set of contiguous blocks on the device.
* Defined by the address of the first block and length (in block units).
* Consequences: External fragmentation and need for compaction.

## Free-space management

* System maintains a **free-space list** to track all free device blocks.
* When creating a file, space is allocated from this list.
* When deleting a file, its space is added back to the list.

### Bit vector (bitmap)

* Common implementation of the free-space list.
* Each block is represented by 1 bit: 1 for free, 0 for allocated.
* Advantage: Simplicity and efficiency in finding first free block or `n` consecutive free blocks.
    * Example: `001111001111110001100000011100000 ...` (where 1s are free blocks)

## Recovery

Methods to deal with file system corruption:

* **Consistency checker (e.g., `fsck` in UNIX):** Compares data in directory structure and metadata with storage state, attempts to fix inconsistencies.
* **Log-based transaction-oriented (journaling) file systems:** Use recovery algorithms based on a log of transactions in stable storage.
* **Backup-and-restore:** Restoring from a previous backup.

## Network file systems (NFS)

* Integrated with the client system's overall directory structure and interface.
* Widely used client-server network file system (part of ONC+).
* **Mount operation:**
    * A remote directory is made accessible transparently from a local machine.
    * Involves mounting a remote directory over a local file system directory.
    * Once mounted, it looks like an integral subtree of the local file system.
    * Location (host name) of the remote directory must be provided during mount.
    * Users then access files transparently.
* Any file system or directory can be mounted remotely on any local directory.
* Diskless workstations can mount roots from servers.
* **Cascading mounts:** A file system can be mounted over another remotely mounted (not local) file system.
* **Non-transitivity:** Mounting a remote file system does not give access to other file systems that were incidentally mounted over the former.
