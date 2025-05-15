# Simple Memory Allocator (C++)

A basic memory allocator implemented in C++ without using `malloc` or `free`. It uses a fixed-size memory pool and a free list to manage memory manually.

## Features

- Fixed-size memory pool (1MB)
- Manual memory allocation (`my_malloc`)
- Manual memory deallocation (`my_free`)
- Free list with block splitting and coalescing
- Memory alignment to 8 bytes

## Build Instructions

Compile using a C++17-compatible compiler:

```bash
g++ -std=c++17 -o allocator main.cpp