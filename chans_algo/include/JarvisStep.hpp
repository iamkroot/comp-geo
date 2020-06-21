#pragma once
#include <bits/stdc++.h>
#include "Point.hpp"

using namespace std;

class JarvisStep
{
    /**
     * @brief Number of points
     */
    int numPoints;

    /**
     * @brief Next point in jarvis march
     */
    Point nextPoint;

public:
    /**
     * @brief Construct a new Jarvis March object
     * 
     * @param pivot Reference pivot from which points are compared
     * @param points Set of candidate points for Convex Hull
     */
    JarvisStep(Point pivot, vector<Point> &points);

    /**
     * @brief Get the next point in jarvis march
     * 
     * @return Point 
     */
    Point getNext();
};