# Cache and Matrix Transpose Optimization Project

## Overview
This project involved analyzing cache behavior, inferring cache parameters, and optimizing a matrix transpose function to minimize cache misses. Through careful implementation and testing, efficient solutions were developed for both tasks, contributing to improved understanding of cache optimization techniques and their practical applications in computer architecture.

### Part 1: Cache Parameter Inference
In this part, the goal is to determine the parameters (block size, cache size, and associativity) of mystery caches by analyzing their behavior using a provided simulator. The cache-test-skel.c file contains skeleton code for this task. By linking this code with different cache object files and observing cache accesses, the cache parameters are inferred.

### Part 2: Matrix Transpose Optimization
The second part focuses on optimizing the performance of a matrix transpose function. The provided transpose function, `trans()`, has a suboptimal cache access pattern leading to numerous cache misses. The task is to implement an optimized version called `transpose_submit()` that minimizes cache misses for different matrix sizes.

## Main Files Edited
- **Part 1:**
    - `cache-test-skel.c`: This file contains the skeleton code for determining cache parameters. I completed the functions within this file to analyze cache behavior and infer cache parameters.

- **Part 2:**
    - `trans.c`: This file contains implementations of the matrix transpose functions. I optimized the `transpose_submit()` function to minimize cache misses while adhering to provided programming rules.


 
