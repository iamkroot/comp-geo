#include <bits/stdc++.h>
#include "GrahamScan.hpp"

using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<Point> points;
    for(int i = 1; i <= n; i++)
    {
        int x, y;
        cin >> x >> y;
        points.push_back(Point(x, y));
    }
    GrahamScan(points).getRightTangentPoint(Point(990, 614)).print();
    return 0;
}