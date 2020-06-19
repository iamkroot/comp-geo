#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <iostream>
#include "common.hpp"

template<typename PrecisionT = float>
struct Point2D {
    PrecisionT x, y;

    Point2D() : x(0), y(0) {}

    Point2D(PrecisionT x, PrecisionT y) : x(x), y(y) {}

    PrecisionT dist_sq(Point2D other) {
        PrecisionT dx = x - other.x, dy = y - other.y;
        return dx * dx + dy * dy;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point2D &d) {
        os << "(" << d.x << ", " << d.y << ")";
        return os;
    }

    bool operator<(const Point2D &rhs) const {
        if (approx_eq(y, rhs.y)) {
            if (approx_eq(x, rhs.x))
                return false;
            else
                return x < rhs.x;
        } else {
            return y > rhs.y;
        }
    }

    bool operator>(const Point2D &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Point2D &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Point2D &rhs) const {
        return !(*this < rhs);
    }

    bool operator==(const Point2D &rhs) const {
        return approx_eq(x, rhs.x) and approx_eq(y, rhs.y);
    }

    bool operator!=(const Point2D &rhs) const {
        return !(rhs == *this);
    }

    Point2D operator+(const Point2D &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    Point2D operator-(const Point2D &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    /**
     * @brief 2D cross product
     * @return
     */
    PrecisionT cross(const Point2D &rhs) const {
        return x * rhs.y - y * rhs.x;
    }

    /**
     * @brief Scalar multiplication
     * @return
     */
    Point2D operator*(const PrecisionT &scalar) const {
        return {x * scalar, y * scalar};
    }

    PrecisionT dot(const Point2D &rhs) const {
        return x * rhs.x + y * rhs.y;
    }
};

#endif //POINT2D_HPP
