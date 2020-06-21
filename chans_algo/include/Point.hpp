#pragma once
#include "Utils.hpp"

using namespace std;

template<class T>
class Point
{

public:
    T x, y;

    Point()
    {
        x = 0;
        y = 0;
    }

    /**
     * @brief Construct a new Point object
     * 
     * @param a 
     * @param b 
     */
    Point(T a, T b)
    {
        x = a;
        y = b;
    }

    /**
     * @brief Compare two points, by order of x-coordinate then y-coordinate
     * 
     * @param p 
     * @return true If 'this' point is lesser in ordering
     * @return false otherwise
     */
    bool operator<(const Point &p)
    {
        if (x != p.x)
            return x < p.x;
        return y < p.y;
    }

    /**
     * @brief Check equality of points
     * 
     * @param p 
     * @return true 
     * @return false 
     */
    bool operator==(const Point &p)
    {
        return (x == p.x && y == p.y);
    }

    /**
     * @brief Compute cross product of two vectors (represented as points)
     * 
     * @param p 
     * @return int signed area of the parallelipiped formed by the vectors
     */
    int cross(Point &p)
    {
        return (x * p.y - p.x * y);
    }

    /**
     * @brief Compute orientation of the traversal of three points
     * 
     * @param p1 
     * @param p2 
     * @param p3 
     * @return int value signifying orientation
     */
    static int orient(const Point &p1, const Point &p2, const Point &p3)
    {
        Point v1(p2.x - p1.x, p2.y - p1.y);
        Point v2(p3.x - p2.x, p3.y - p2.y);
        int o = v1.cross(v2);
        if (o > 0)
            return 1;
        else if (o == 0)
            return 0;
        else
            return -1;
    }

    /**
     * @brief Compute squared Euclidean distance between points
     * 
     * @param p 
     * @return int distance
     */
    int squaredDistance(Point &p)
    {
        return Utils<T>::square(x - p.x) + Utils<T>::square(y - p.y);
    }

    /**
     * @brief Utility to print the point
     * 
     */
    void print()
    {
        cout << x << " " << y << endl;
    }
};