# POKS Lab - Operating Systems Course

This repository contains lab exercises and projects from the POKS (Programska Oprema Komunikacijskih Sistemov) course.

## Structure

### Lab 2 - C Fundamentals
Basic C programming exercises covering:
- Variables and data types
- Control flow and loops
- Functions and arrays
- Input/output operations

**Files:** `nal1.c`, `nal2.c`, `nal3.c`, `nal4.c`

### Lab 3 - Intermediate C
Intermediate C concepts including:
- Pointers and memory management
- Structures and unions
- File I/O operations
- String manipulation

**Files:** `main.c`, `nal2.c`, `nal3.c`, `nal4.c`, `nal5.c`

### Lab 4 - Multi-threading
Introduction to POSIX threads (pthreads):
- Thread creation and management
- Thread synchronization
- Passing arguments to threads
- Thread return values
- Concurrent calculations with multiple threads

**Files:** `nal1.c`, `nal1b.c`, `nal2.c`, `nal3.c`

*Example: `nal1b.c` demonstrates calculating R = (A + B) * (C - D) * (E + F) using three separate threads*

### Lab 5 - Semaphores
Semaphore-based synchronization:
- Binary semaphores
- Counting semaphores
- Producer-consumer problem
- Critical section protection

**Files:** `nal1.c`, `nal2.c`, `nal3.c`

### Lab 6 - Kernel Modules
Linux kernel module development:
- Module compilation using Makefiles
- Module initialization and cleanup
- Kernel module parameters
- Device interaction

**Files:** `hello.c`, `Makefile`
**Subdir:** `POKS-MODULES/` (additional module examples)

### Lab 7 - Circular Buffer Queue
Inter-process communication and synchronization:
- Circular buffer implementation
- Producer-consumer patterns
- Message passing between processes
- Synchronization mechanisms

**Files:** `common.h`, `sender.c`, `sender2.c`, `reciever.c`, `reciever2.c`, `reciever3.c`

### Lab 8 - Socket Programming
Network communication using sockets and inter-process communication:
- Socket creation and configuration
- TCP (stream sockets) and UDP (datagram sockets) protocols
- Client-server model implementation
- Network addressing using IP addresses and ports
- Data transmission and reception over networks
- SIP protocol implementation using UDP sockets

**Key Concepts:**

**TCP Server Steps:**
1. Socket creation with `socket()`
2. Binding socket to address with `bind()`
3. Listening for connections with `listen()`
4. Accepting connections with `accept()`
5. Reading/writing data with `read()`, `write()`

**TCP Client Steps:**
1. Socket creation with `socket()`
2. Connecting to server with `connect()`
3. Reading/writing data with `read()`, `write()`

**UDP Operations:**
- Server uses `recvfrom()` for receiving datagrams
- Client/Server uses `sendto()` for sending datagrams
- No listening or accepting required (connectionless)

**Network Tools:**
- `netstat -lnup` - monitor socket connections
- `tcpdump -i eth0 udp` - trace UDP packet flow

**Files:** UDP server and client implementations with exercises for local and inter-machine communication

### Project - Final Assignment
Complete application implementing:
- Multiple processes/threads
- Advanced synchronization
- Resource management
- Real-world problem solving

**Files:** `Projekt_1.c`, `Projekt_2.c`

## Compilation

Most C programs can be compiled with:
```bash
gcc -o output_name source_file.c
```

For programs using pthreads, link the pthread library:
```bash
gcc -o output_name source_file.c -lpthread
```

For kernel modules, use the provided Makefile:
```bash
make
```

## Documentation

Each lab directory includes PDF documentation files with detailed instructions and theoretical background in Slovene language.

## Learning Objectives

- Master C programming fundamentals
- Understand process and thread management
- Implement synchronization mechanisms
- Develop kernel modules for Linux
- Design inter-process communication systems
- Apply operating system concepts to practical problems

---

*Course: POKS (Operating Systems)*
