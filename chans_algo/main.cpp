#include <bits/stdc++.h>
#include "Chan.hpp"
#include <vector>

using namespace std;
using namespace std::chrono;

int32_t main()
{
    // Number of points in the plane
    int n;
    cin >> n;
    vector<Point<int>> points;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        points.push_back(Point<int>(x, y));
    }
    auto tic = high_resolution_clock::now();
    vector<Point<int>> convexHull = Chan<int>(points).getConvexHull();
    auto toc = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(toc - tic);

    cout << duration.count() << endl;
    /*
    cout << "Counter Clockwise order of Convex Hull: " << endl;
    for (Point<int> &point : convexHull)
    {
        cout << point.x << " " << point.y << endl;
    }
    */
}