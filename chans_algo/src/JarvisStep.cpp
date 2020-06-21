#include "JarvisStep.hpp"
#include "Utils.hpp"

JarvisStep::JarvisStep(Point pivot, vector<Point> &points)
{
    Point best(Utils::INF, Utils::INF);
    for (Point &point : points)
    {
        if (point == pivot)
            continue;
        if (best == Point(Utils::INF, Utils::INF))
        {
            best = point;
        }
        else
        {
            // Selecting the right-most-oriented point w.r.t pivot
            int orientation = pivot.orient(pivot, best, point);
            if (orientation < 0)
                best = point;
            else if (orientation == 0)
            {
                // If points are collinear take the farthest one
                if (pivot.squaredDistance(point) > pivot.squaredDistance(best))
                    best = point;
            }
        }
    }
    nextPoint = best;
}

Point JarvisStep::getNext()
{
    return nextPoint;
}
