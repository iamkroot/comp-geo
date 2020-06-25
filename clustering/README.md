# Approximate Algorithm for Clustering

Implementation of the 2-approximate algorithm for solving the k-center problem.

## The k-center problem
Given a set P of n points in space and an integer k ≤ n, find a set C ⊆ P of k points in order to minimize the maximum distance of any point of P to its closest center in C. 

Stated mathematically, the problem is
min max_p∈P min_c∈C δ(p,c)

Here, the distance δ(u,v) between two points u and v should be a "metric" distance, which satisfies the following properties:
* Postivity: δ(u,v) ≥ 0 and δ(u,v) = 0 if and only if u = v
* Symmetry: δ(u,v) = δ(v,u)
* Triangle inequality: δ(u,w) ≤ δ(u,v) + δ(v,w)

## Greedy Approximation Algorithm
The problem stated above is known to be NP-hard. Here, we have implemented an approximation algorithm for the same that produces an estimate that is at most twice as large as the optimum value, as described by [Gonzalez](https://www.sciencedirect.com/science/article/pii/0304397585902245). It works by greedily picking a point p ∈ (P \ C) such that p is the point furthest from all the centers currently in C.

### Overview
1. Initially, we start by selecting an arbitrary point as the first center and add it to our set of centers C.
2. Then, we select the point that is furthest from the center to be the next center.
3. Step 2 is repeated until |C| becomes k, or there are no more points left (in case |P| < k) 

## Code

```cpp
auto greedyKCenter(const std::vector<Point2D> &points, int k) {  // Point2D is a struct defined previously
    std::vector<Point2D> centers;
    std::vector<int> centerLabels(points.size());  // represents the cluster index for each point
    std::vector<float> dist(points.size(), std::numeric_limits<float>::max());  // initialize the distances as infinity

    for (int i = 0; i < k; ++i) {  // O(k * n)
    	// select the point with maximum dist value to be the next center
        Point2D center = *std::max_element(dist.begin(), dist.end());  // O(n)
        centers.push_back(center);
    	// calculate the distance of each point from the current center 
        for (int j = 0; j < points.size(); ++j) {  // O(n)
            float d = center.dist(points[j]);
            if (d < dist[j]) {
            	// point is closer to the new center than any previous center
                dist[j] = d;
                centerLabels[j] = i;
            }
        }
    }
    return std::make_tuple(centers, centerLabels, dist);
}
```

## Analysis and Approximation Bound
It is quite clear that the algorithm runs in O(nk) time. So this is O(n^2) in the worst case (for k = n).

For a distance measure that satisfies the metric properties as defined previously, it can be proven that this algorithm gives a solution for the k-center problem whose cost (maximum distance of a point from its closest center) is at most twice that of the optimal solution O.

## Experimentation



## Results
To measure the running time of the algorithm, we used `high_resolution_clock` from `std::chrono` in C++. Refer to the table to see the runtimes.

num_points(n)|num_centers(k)|runtime(us)
---|---|---
200|10|141
200|20|221
200|50|533
500|10|432
500|20|488
500|50|2063
1000|10|959
1000|20|1694
1000|50|5577
2000|10|2201
2000|20|1970
2000|50|8214
2000|100|8968
2000|200|18050
2000|500|43021
5000|10|3404
5000|20|4571
5000|50|19553
5000|100|26671
5000|200|39628
5000|500|102654
10000|10|7097
10000|20|11143
10000|50|28964
10000|100|63711
10000|200|94761
10000|500|196955
20000|10|9412
20000|20|24174
20000|50|45722
20000|100|80913
20000|200|234522
20000|500|405571
20000|1000|916742
20000|2000|1587635
20000|5000|3757584
50000|10|20212
50000|20|62195
50000|50|88471
50000|100|213662
50000|200|449861
50000|500|1224021
50000|1000|2034864
50000|2000|3651230
50000|5000|8453290
100000|10|34729
100000|20|119031
100000|50|236493
100000|100|668943
100000|200|885612
100000|500|1762303
100000|1000|3522945
100000|2000|7014360
100000|5000|16952356
200000|10|83506
200000|20|163286
200000|50|381765
200000|100|733378
200000|200|1485523
200000|500|3490911
200000|1000|6724079
200000|2000|13048784
200000|5000|35032701
200000|10000|70862785
200000|20000|134255857

**Figure 1** [n vs t Interactive](https://plotly.com/~userisro/150/)
**Figure 2** [k vs t Interactive](https://plotly.com/~userisro/148/)

### Interpretation
The results from our experimentation match excellently with the theoretical bounds of the algorithm.
* It is evident from figure 1 that the running time of the algorithm is linearly proportional to the number of points, for various values of k 
* Similarly, figure 2 shows that the running time is also linearly proportional to the number of centers, when n is fixed

Thus we can see that t prop to n for constant k and t prop to k for constant n and hence t prop to nk.

### Conclusion

The metric k-center problem is proven to be NP-Hard. We can relax the optimality constraint in order to find efficient solutions for the problem. The greedy algorithm described above is one such approach that is able to find an approximate solution guaranteed to generate a cost at most twice the optimal cost. We validated the theoretical bound of O(nk) for the running time of the algorithm by testing on various datasets.
