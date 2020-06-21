#include "GrahamScan.hpp"

using namespace std;

GrahamScan::GrahamScan(vector<Point> &points)
{
    numPoints = points.size();
    assert(numPoints >= 3);

    Point best(Utils::INF, Utils::INF);
    for(Point &point: points)
        if (point < best)
            best = point;

    // Sort vertices by x-coordinate then by y-coordinate
    sort(points.begin(), points.end(), Comparator(best));

    order.push(points[0]);
    order.push(points[1]);

    // Construct Convex hull
    for (int i = 2; i < numPoints; i++)
        insert(points[i]);

    // Get all points in convex hull avoiding repeatition for lowermost-leftmost point
    while (!order.empty())
    {
        Point top = order.top();
        convexHull.push_back(top);
        order.pop();
    }
    reverse(convexHull.begin(), convexHull.end());
    convexHullSize = convexHull.size();
}

void GrahamScan::insert(Point &p)
{
    Point top = order.top();
    order.pop();
    while (order.size() >= 1 && Point::orient(order.top(), top, p) <= 0)
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

bool GrahamScan::isAbove(Point pivot, Point a, Point b)
{
    if (Point::orient(pivot, a, b) > 0)
        return true;
    else if (Point::orient(pivot, a, b) == 0)
    {
        if (pivot.squaredDistance(a) > pivot.squaredDistance(b))
            return true;
    }
    return false;
}

bool GrahamScan::isBelow(Point pivot, Point a, Point b)
{
    if (Point::orient(pivot, a, b) < 0)
        return true;
    else if (Point::orient(pivot, a, b) == 0)
    {
        if (pivot.squaredDistance(a) < pivot.squaredDistance(b))
            return true;
    }
    return false;
}

Point GrahamScan::getRightTangentPoint(Point pivot)
{
    if (convexHullSize < 3)
        return JarvisStep(pivot, convexHull).getNext();

    // Check if convexHull[0] if local maximum
    if (isBelow(pivot, getPoint(1), getPoint(0)) &&
        !isAbove(pivot, getPoint(convexHullSize - 1), getPoint(0)))
        return getPoint(0);

    int low = 0, high = convexHullSize;
    while (true)
    {
        int mid = (low + high) / 2;
        bool isMidDown = isBelow(pivot, getPoint(mid + 1), getPoint(mid));
        if (isMidDown && !isAbove(pivot, getPoint(mid - 1), getPoint(mid)))
            return getPoint(mid);
        bool isLowUp = isAbove(pivot, getPoint(low + 1), getPoint(low));
        if (isLowUp)
        {
            if (isMidDown)
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
            if (!isMidDown)
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

Point GrahamScan::getPoint(int idx)
{
    if (idx >= convexHullSize)
        idx -= convexHullSize;
    else if (idx < 0)
        idx += convexHullSize;
    return convexHull[idx];
}