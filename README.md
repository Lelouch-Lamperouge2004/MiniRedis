# MiniRedis

A lightweight Redis-inspired in-memory cache server built in C++ with support for LRU eviction, TTL expiration, persistence, TCP networking, concurrency, benchmarking, and automated testing.

---

## Features

### Cache Engine

* O(1) GET operation
* O(1) SET operation
* O(1) DELETE operation
* HashMap + Doubly Linked List architecture

### LRU Eviction

* Least Recently Used replacement policy
* Automatic eviction when capacity is reached

### TTL Support

* Key expiration using:

```
SET key value EX seconds
```

* Automatic cleanup of expired entries

### Persistence

* Cache saved to disk
* Cache restored on restart
* TTL information preserved

### Thread Safety

* Mutex-protected cache operations
* Safe concurrent access from multiple clients

### TCP Server

* WinSock based TCP server
* Listens on port 6379
* Handles multiple client connections

### Benchmarking

* Performance measurement command

```
BENCHMARK
```

### Testing

* Google Test integration
* Automated unit tests

---

## Architecture

```
Client
   |
TCP Socket
   |
TCP Server
   |
Command Processor
   |
LRU Cache
   |
+------------------+
| HashMap          |
| Doubly LinkedList|
+------------------+
   |
Persistence Layer
   |
cache.dat
```

---

## Project Structure

```
MiniRedis/
│
├── src/
│   ├── Node.cpp
│   ├── Node.h
│   ├── DoublyLinkedList.cpp
│   ├── DoublyLinkedList.h
│   ├── LRUCache.cpp
│   ├── LRUCache.h
│   ├── PersistenceManager.cpp
│   ├── PersistenceManager.h
│   ├── TCPServer.cpp
│   ├── TCPServer.h
│   ├── CommandParser.cpp
│   └── main.cpp
│
├── tests/
│   ├── cache_test.cpp
│   └── stress_test.cpp
│
├── data/
│   └── cache.dat
│
└── CMakeLists.txt
```

---

## Supported Commands

### SET

```
SET name Dazai
```

Response:

```
OK
```

---

### GET

```
GET name
```

Response:

```
Dazai
```

---

### DELETE

```
DEL name
```

Response:

```
Deleted
```

---

### EXISTS

```
EXISTS name
```

Response:

```
true
```

---

### SIZE

```
SIZE
```

Response:

```
3
```

---

### TTL

```
TTL otp
```

Response:

```
55
```

---

### CLEAR

```
CLEAR
```

Response:

```
Cache Cleared
```

---

### SAVE

```
SAVE
```

Response:

```
Cache Saved
```

---

### SHUTDOWN

```
SHUTDOWN
```

Response:

```
Server shutting down
```

---

### BENCHMARK

```
BENCHMARK
```

Example:

```
Benchmark completed:
100000 SET and 100000 GET operations in 98 ms
```

---

## Build Instructions

### Prerequisites

* C++17
* CMake
* MinGW
* MSYS2
* Google Test

### Build

```bash
mkdir build
cd build

cmake .. -G "MinGW Makefiles"

mingw32-make
```

---

## Running

### Start Server

```bash
MiniRedis.exe
```

Output:

```
MiniRedis TCP Server listening on port 6379
```

---

## Unit Tests

Run:

```bash
MiniRedisTests.exe
```

Current coverage:

* SET
* GET
* DELETE
* LRU Eviction
* TTL Expiration
* EXISTS
* SIZE
* CLEAR
* Persistence

---

## Stress Testing

Run:

```bash
MiniRedisStressTest.exe
```

Verified:

* 50 concurrent clients
* 200 concurrent clients

---

## Benchmark Results

System dependent.

Example run:

```
100000 SET operations
100000 GET operations

Total Time: 98 ms
Throughput: ~2 million operations/sec
```

---

## Time Complexity

| Operation | Complexity |
| --------- | ---------- |
| GET       | O(1)       |
| SET       | O(1)       |
| DELETE    | O(1)       |
| EXISTS    | O(1)       |
| SIZE      | O(1)       |
| TTL       | O(1)       |

---

## Technologies Used

* C++
* STL
* WinSock2
* Multithreading
* Mutex
* CMake
* Google Test
* TCP/IP Networking

---

## Future Improvements

* RESP protocol support
* Redis-compatible client support
* Connection pooling
* Snapshot persistence
* Asynchronous I/O
* Replication
* Distributed caching

---

## Author

Developed as a systems programming and networking project demonstrating:

* Data Structures
* Operating Systems Concepts
* Concurrency
* Networking
* System Design
* Performance Engineering
