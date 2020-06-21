#include "Chan.hpp"

// Allowed type bindings
template class Chan<int>;
template class Chan<long>;
template class Chan<long long>;
template class Chan<float>;
template class Chan<double>;

template <class T>
Chan<T>::Chan(vector<Point<T>> points)
{
    numPoints = points.size();
    this->points = points;
    assert(numPoints >= 3);

    // Compute pivot
    Point<T> best(points[0]);
    for (Point<T> &point : points)
    {
        if (point < best)
            best = point;
    }
    pivot = best;

    int guessedHullSize = 3;
    while (!restrictedConvexHull(guessedHullSize))
        guessedHullSize = guessedHullSize * guessedHullSize;
}

template <class T>
bool Chan<T>::restrictedConvexHull(int guessedHullSize)
{
    // Compute partitions
    vector<vector<Point<T>>> partitions = computePartitions(guessedHullSize);

    // Graham's step O(numPoints * log(guessedHullSize))
    vector<GrahamScan<T>> grahamScans;
    for (vector<Point<T>> &partition : partitions)
        grahamScans.push_back(GrahamScan<T>(partition));

    Point<T> currPivot = pivot;
    vector<Point<T>> hull;
    for (int i = 1; i <= guessedHullSize; i++)
    {
        hull.push_back(currPivot);
        vector<Point<T>> candidatePoints;
        for (GrahamScan<T> &grahamScan : grahamScans)
            candidatePoints.push_back(grahamScan.getRightTangentPoint(currPivot));

        // Perform jarvis step
        currPivot = JarvisStep<T>(currPivot, candidatePoints).getNext();
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

template <class T>
vector<vector<Point<T>>> Chan<T>::computePartitions(int partitionSize)
{
    vector<vector<Point<T>>> partitions;
    vector<Point<T>> partition;
    if (partitionSize >= numPoints)
    {
        partitions.push_back(points);
        return partitions;
    }
    int numPartitions = numPoints / partitionSize;
    partitions = vector<vector<Point<T>>>(numPartitions);
    for (int i = 0; i < numPoints; i++)
        partitions[i % numPartitions].push_back(points[i]);
    return partitions;
}

template <class T>
vector<Point<T>> Chan<T>::getConvexHull()
{
    return convexHull;
}