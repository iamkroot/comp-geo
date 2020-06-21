#include "Chan.hpp"

Chan::Chan(vector<Point> points)
{
    numPoints = points.size();
    this->points = points;
    assert(numPoints >= 3);

    // Compute pivot
    Point best(Utils::INF, Utils::INF);
    for (Point &point : points)
    {
        if (point < best)
            best = point;
    }
    pivot = best;

    int guessedHullSize = 3;
    while (!restrictedConvexHull(guessedHullSize))
        guessedHullSize = guessedHullSize * guessedHullSize;
}

bool Chan::restrictedConvexHull(int guessedHullSize)
{
    // Compute partitions
    vector<vector<Point>> partitions = computePartitions(guessedHullSize);

    // Graham's step O(numPoints * log(guessedHullSize))
    vector<GrahamScan> grahamScans;
    for (vector<Point> &partition : partitions)
        grahamScans.push_back(GrahamScan(partition));

    Point currPivot = pivot;
    vector<Point> hull;
    for (int i = 1; i <= guessedHullSize; i++)
    {
        hull.push_back(currPivot);
        vector<Point> candidatePoints;
        for (GrahamScan &grahamScan : grahamScans)
        {
            pair<Point, Point> tangentPoints = grahamScan.getTangentPoints(currPivot);
            candidatePoints.push_back(tangentPoints.first);
            candidatePoints.push_back(tangentPoints.second);
        }
        // Perform jarvis step
        currPivot = JarvisStep(currPivot, candidatePoints).getNext();
        if (currPivot == pivot)
            break;
    }
    if (currPivot == pivot)
    {
        // Finished successfully
        convexHull = hull;
        return true;
    }
    else
    {
        // Failed
        return false;
    }
}

vector<vector<Point>> Chan::computePartitions(int partitionSize)
{
    vector<vector<Point>> partitions;
    vector<Point> partition;
    if (partitionSize >= numPoints)
    {
        partitions.push_back(points);
        return partitions;
    }
    int numPartitions = numPoints / partitionSize;
    partitions = vector<vector<Point>>(numPartitions);
    for (int i = 0; i < numPoints; i++)
        partitions[i % numPartitions].push_back(points[i]);
    return partitions;
}

vector<Point> Chan::getConvexHull()
{
    return convexHull;
}