#pragma once
#include "GrahamScan.hpp"
#include "JarvisStep.hpp"

class Chan
{
    /**
     * @brief Input points
     */
    vector<Point> points;

    /**
     * @brief Set of points in the convex hull in counter clockwise order
     */
    vector<Point> convexHull;

    /**
     * @brief Number of points in the input
     */
    int numPoints;

    /**
     * @brief Convex Hull size
     */
    int convexHullSize;

    /**
     * @brief Pivot point in input points
     */
    Point pivot;

    /**
     * @brief Tries to compute convex hull using given partition size (guessed size of hull)
     * 
     * @param guessedHullSize Guessed size of hull
     * @return true If algorithm completes, i.e. guessed size if greater or equal to actual hull size
     * @return false If algorithm fails to complete
     */
    bool restrictedConvexHull(int guessedHullSize);

    /**
     * @brief Divide input points equally into partitions of size 'partitionSize'
     * 
     * @param partitionSize 
     * @return vector<vector<Point>> Partitions
     */
    vector<vector<Point>> computePartitions(int partitionSize);

public:
    /**
     * @brief Construct a new Chan object
     * 
     * @param points Set of input points
     */
    Chan(vector<Point> points);

    /**
     * @brief Get convex hull computed using Chan's algorithm
     * 
     * @return vector<Point> Set of points int convex hull in counter clockwise order
     */
    vector<Point> getConvexHull();
};