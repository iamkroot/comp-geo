#include <bits/stdc++.h>
#include "Point.hpp"
#include "GrahamScan.hpp"

using namespace std;

GrahamScan::GrahamScan(vector<Point> &points)
{
    numPoints = points.size();
    assert(numPoints >= 3);
    
    // Sort vertices by x-coordinate then by y-coordinate
    sort(points.begin(), points.end());

    order.push(points[0]);
    order.push(points[1]);

    // Construct Upper Hull
    for (int i = 2; i < numPoints; i++)
        insert(points[i]);

    order.push(points[numPoints - 2]);
    // Construct the Lower Hull
    for (int i = numPoints - 3; i >= 0; i--)
        insert(points[i]);

    // Get all points in convex hull avoiding repeatition for lowermost-leftmost point
    while (order.size() > 1)
    {
        Point top = order.top();
        convexHull.push_back(top);
        order.pop();
    }
    convexHullSize = convexHull.size();
}

void GrahamScan::insert(Point &p)
{
    Point top = order.top();
    order.pop();
    while (order.size() >= 1 && Point::orient(order.top(), top, p) >= 0)
    {
        top = order.top();
        order.pop();
    }
    order.push(top);
    order.push(p);
}

vector<Point> GrahamScan::getConvexHull()
{
    return convexHull;
}

pair<Point, Point> GrahamScan::getTangentPoints(Point pivot)
{
    Point leftTangentPoint = getLeftTangentPoint(pivot);
    Point rightTangetPoint = getRightTangentPoint(pivot);
    return {leftTangentPoint, rightTangetPoint};
}

bool GrahamScan::isAbove(Point pivot, Point a, Point b)
{
    if (Point::orient(pivot, a, b) > 0)
        return true;
    return false;
}

bool GrahamScan::isBelow(Point pivot, Point a, Point b)
{
    if (Point::orient(pivot, a, b) < 0)
        return true;
    return false;
}

Point GrahamScan::getRightTangentPoint(Point pivot)
{
    if (getPoint(0) == pivot)
        return getPoint(1);
    if (getPoint(1) == pivot)
        return getPoint(2);
    if (getPoint(convexHullSize - 1) == pivot)
        return getPoint(convexHullSize);

    // Check if convexHull[0] if local maximum
    if (isBelow(pivot, getPoint(1), getPoint(0)) &&
        !isAbove(pivot, getPoint(convexHullSize - 1), getPoint(0)))
        return getPoint(0);

    int low = 0, high = convexHullSize;
    while (true)
    {
        int mid = (low + high) / 2;
        if (getPoint(low) == pivot)
            return getPoint(low + 1);
        if (getPoint(low + 1) == pivot)
            return getPoint(low + 2);
        if (getPoint(mid + 1) == pivot)
            return getPoint(mid + 2);
        if (getPoint(mid) == pivot)
            return getPoint(mid + 1);
        if (getPoint(mid - 1) == pivot)
            return getPoint(mid);
        bool isDownEdge = isBelow(pivot, getPoint(mid + 1), getPoint(mid));
        if (isDownEdge && !isAbove(pivot, getPoint(mid - 1), getPoint(mid)))
            return getPoint(mid);
        if (isAbove(pivot, getPoint(low + 1), getPoint(low)))
        {
            if (isDownEdge)
                high = mid;
            else
            {
                if (isAbove(pivot, getPoint(low), getPoint(mid)))
                    high = mid;
                else
                    low = mid;
            }
        }
        else
        {
            if (!isDownEdge)
                low = mid;
            else
            {
                if (isBelow(pivot, getPoint(low), getPoint(mid)))
                    high = mid;
                else
                    low = mid;
            }
        }
    }
}

Point GrahamScan::getLeftTangentPoint(Point pivot)
{
    if (getPoint(0) == pivot)
        return getPoint(-1);
    if (getPoint(1) == pivot)
        return getPoint(0);
    if (getPoint(convexHullSize - 1) == pivot)
        return getPoint(convexHullSize - 2);

    // Check if convexHull[0] if local minimum
    if (isAbove(pivot, getPoint(convexHullSize - 1), getPoint(0)) &&
        !isBelow(pivot, getPoint(1), getPoint(0)))
        return getPoint(0);

    int low = 0, high = convexHullSize;
    while (true)
    {
        int mid = (low + high) / 2;
        if (getPoint(low) == pivot)
            return getPoint(low - 1);
        if (getPoint(low + 1) == pivot)
            return getPoint(low);
        if (getPoint(mid + 1) == pivot)
            return getPoint(mid);
        if (getPoint(mid) == pivot)
            return getPoint(mid - 1);
        if (getPoint(mid - 1) == pivot)
            return getPoint(mid - 2);
        bool isDownEdge = isBelow(pivot, getPoint(mid + 1), getPoint(mid));
        if (isAbove(pivot, getPoint(mid - 1), getPoint(mid)) && !isDownEdge)
            return getPoint(mid);
        if (isBelow(pivot, getPoint(low + 1), getPoint(low)))
        {
            if (!isDownEdge)
                high = mid;
            else
            {
                if (isBelow(pivot, getPoint(low), getPoint(mid)))
                    high = mid;
                else
                    low = mid;
            }
        }
        else
        {
            if (isDownEdge)
                low = mid;
            else
            {
                if (isAbove(pivot, getPoint(low), getPoint(mid)))
                    high = mid;
                else
                    low = mid;
            }
        }
    }
}

Point GrahamScan::getPoint(int idx)
{
    if (idx >= convexHullSize)
        idx -= convexHullSize;
    else if (idx < 0)
        idx += convexHullSize;
    return convexHull[idx];
}