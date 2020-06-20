#pragma once
#include "Utils.hpp"

using namespace std;

class Point
{

public:
    int x, y;

    Point()
    {
        x = 0;
        y = 0;
    }

    Point(int a, int b)
    {
        x = a;
        y = b;
    }

    bool operator<(const Point &p)
    {
        if (x != p.x)
            return x < p.x;
        return y < p.y;
    }

    bool operator==(const Point &p)
    {
        return (x == p.x && y == p.y);
    }

    int cross(Point &p)
    {
        return (x * p.y - p.x * y);
    }

    // Finds the orientation of the given 3 points
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

    int squaredDistance(Point &p)
    {
        return Utils::square(x - p.x) + Utils::square(y - p.y);
    }

    void print()
    {
        cout << x << " " << y << endl;
    }
};