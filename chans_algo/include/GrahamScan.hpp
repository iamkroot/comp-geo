#pragma once
#include <bits/stdc++.h>
#include "Point.hpp"
#include "JarvisStep.hpp"

using namespace std;

class GrahamScan
{
    /**
     * @brief Order stack for maintaining points in the partially
     * constructed convex hull
     */
    stack<Point> order;

    /**
     * @brief Insert new point into the stack of partially constructed convex hull
     * 
     * @param p New point to be inserted
     */
    void insert(Point &p);

    /**
     * @brief Number of points in input
     */
    int numPoints;

    /**
     * @brief Counter clockwiser order of points in the convex hull
     */
    vector<Point> convexHull;

    /**
     * @brief Determine if 'a' lies strictly to left of 'b' (in orientation) w.r.t pivot
     * 
     * @param pivot 
     * @param a 
     * @param b 
     * @return true If 'a' lies strictly to left of 'b' 
     * @return false otherwise
     */
    bool isBelow(Point pivot, Point a, Point b);

    /**
     * @brief Determine if a lies strictly to right of b (in orientation) w.r.t pivot
     * 
     * @param pivot 
     * @param a 
     * @param b 
     * @return true If 'a' lies strictly to right of b
     * @return false otherwise
     */
    bool isAbove(Point pivot, Point a, Point b);

    /**
     * @brief Number of points in convex hull
     */
    int convexHullSize;

    /**
     * @brief Get the Point from convex hull at position 'idx'
     * 
     * @param idx 
     * @return Point 
     */
    Point getPoint(int idx);

    /**
     * @brief Compare ordering of points for graham's scan
     */
    class Comparator
    {
        Point pivot;

    public:

        Comparator(Point pivot)
        {
            this->pivot = pivot;
        }

        bool operator()(Point &p1, Point &p2)
        {
            int o = Point::orient(pivot, p1, p2);
            if (o == 1)
                return 1;
            else if (o == -1)
                return 0;
            else
                return p1 < p2;
        }
    };

public:
    /**
     * @brief Construct a new Graham Scan object
     * 
     * @param points set of points for constructing convex hull
     */
    GrahamScan(vector<Point> &points);

    /**
     * @return vector<Point> set of points in convex hull in counter clockwise order
     */
    vector<Point> getConvexHull();

    /**
     * @brief Get the Right Tangent Point, using binary search on the convex hull
     * from the pivot as reference
     * 
     * @param pivot 
     * @return Point on the convex hull lying on the right tangent
     */
    Point getRightTangentPoint(Point pivot);
};