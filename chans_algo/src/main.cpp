#include <bits/stdc++.h>
#include "Chan.hpp"

using namespace std;

int32_t main()
{
    // Number of points in the plane
    int n;
    cin >> n;
    vector<Point> points;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        points.push_back(Point(x, y));
    }
    vector<Point> convexHull = Chan(points).getConvexHull();
    cout << "Counter Clockwise order of Convex Hull: " << endl;
    for(Point &point: convexHull)
        cout << point.x << " " << point.y << endl;
}