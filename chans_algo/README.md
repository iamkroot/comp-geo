# Chan's Algorithm

This project aims to implement and test the famous Timothy Chan's algorithm for Convex Hull computation.

## Introduction
### Convex Hull

A convex hull of a given set of points is a minimal convex polygon which contains all the points within it.

### Related Algorithms

Convex Hull computation is a very well studied and important problem in computational geometry. It has many real world applications which makes worthy to be sought after.

Some of the other well known algorithms for convex hull are:
1. Jarvis March (Gift wrapping) algorithm | `O(n * h)`
2. Graham Scan algorithm | `O(n log n)`

Here, `n` is the number of points in the input and `h` is the number of points in the output convex hull.

### Algorithm & Complexity

Chan's algorithm is inspired by both `Graham Scan` and `Jarvis March`. It uses the ideas from both the algorithms to acheive the asymptotically optimal time complexity of `O(n log h)` where `n` is the number of points in the input and `h` is the number of points on the output convex hull.
Here, since the complexity depends on the output hence, it is a output sensitive algorithm.
Note, `h <= n` Hence, Chan's algorithm is atleast as good as `Graham Scan`.

Moreover, if `h \in o(n)` then Chan's algorithms is asymptotically better than `Graham Scan`.

## Experimentation

We test the running time and space complexity of our implementation for `Chan's algorithm` against the well know `Graham Scan`.

*All tests are averaged over 5 iterations.*

### Random datasets

n (Dataset) |   time (s) (Graham Scan)     |  time (s) (Chan's) 
------------|------------------------------|-------------------------
   1e2      |            0.004             |       0.010        
   1e3      |            0.009             |       0.016
   1e4      |            0.035             |       0.046     
   1e5      |            0.120             |       0.209
   1e6      |            0.891             |       1.632
   2e6      |            1.687             |       3.169
   5e6      |            4.165             |       7.832


### Datasets with fixed h

n (Dataset)  |   h    | time (s) (Graham Scan)  |  time (s) (Chan's) 
-------------|--------|-------------------------|----------------------
   1e2       |  1e1   |       0.008             |       0.009        
   1e3       |  1e1   |       0.011             |       0.012
   1e3       |  1e2   |       0.015             |       0.016     
   1e4       |  1e1   |       0.034             |       0.042
   1e4       |  1e2   |       0.034             |       0.048
   1e4       |  1e3   |       0.036             |       0.049
   1e5       |  1e1   |       0.111             |       0.177
   1e5       |  1e2   |       0.117             |       0.207
   1e5       |  1e3   |       0.116             |       0.206
   1e5       |  1e4   |       0.116             |       0.208
   1e6       |  1e1   |       0.703             |       1.203
   1e6       |  1e2   |       0.707             |       1.333
   1e6       |  1e3   |       0.714             |       1.364
   1e6       |  1e4   |       0.708             |       1.361
   1e6       |  1e5   |       0.707             |       1.366

Although Chan's algorithm is asymtotically better than Graham Scan. We can see that running time is better for Graham Scan. However, Chan's algorithm catches up very fast for large `N`.
Moreover, if we run tests for sufficiently large `N` Chan's algorithm will be able to outperform Graham Scan.

On smaller inputs Chan's algo has a comparatively large constant factor which is the reason for poor running times.


