#ifndef LINESEGMENT_HPP
#define LINESEGMENT_HPP

#include <tuple>
#include <ostream>
#include "Point2D.hpp"

template<typename PrecisionT = float>
struct LineSegment {
    Point2D<PrecisionT> top, bottom;
    PrecisionT m;
    bool isVertical{false};
    bool isHorizontal{false};

    LineSegment(const Point2D<PrecisionT> &a, const Point2D<PrecisionT> &b) {
        std::tie(top, bottom) = std::minmax(a, b);
        auto dx = top.x - bottom.x, dy = top.y - bottom.y;
        isVertical = approx_eq(dx, 0);
        isHorizontal = approx_eq(dy, 0);
        m = !isVertical ? (dy / dx) : std::numeric_limits<PrecisionT>::infinity();
    }

    bool contains(const Point2D<PrecisionT> &point, bool strictInterior = false) const {
        if (strictInterior and (point == top or point == bottom)) {
            return false;
        }
        if (isVertical) {
            return approx_eq(point.x, top.x) and approx_in_range(point.y, bottom.y, top.y);
        } else if (isHorizontal) {
            return approx_eq(point.y, top.y) and approx_in_range(point.x, top.x, bottom.x);
        }
        auto tx = (point.x - bottom.x) / (top.x - bottom.x);
        auto ty = (point.y - bottom.y) / (top.y - bottom.y);
        return approx_in_range(tx, 0, 1) and approx_in_range(ty, 0, 1) and approx_eq(tx, ty);
    }

    PrecisionT interpX(const PrecisionT &y) const {
        if (isVertical or isHorizontal)  // this is technically incorrect in case the line is horizontal and y != top.y
            return top.x;
        else
            return (y - bottom.y) / m + bottom.x;
    }

    PrecisionT interpY(const PrecisionT &x) const {
        if (isVertical or isHorizontal)  // this is technically incorrect in case the line is vertical and x != top.x
            return top.y;
        else
            return m * (x - bottom.x) + bottom.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const LineSegment &segment) {
        os << "(" << segment.top << ", " << segment.bottom << ")";
        return os;
    }
};

#endif //LINESEGMENT_HPP
