#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <ostream>
#include "Point2D.hpp"
#include "LineSegment.hpp"

enum class IntersectionType {
    CollinearOverlapping,
    CollinearDisjoint,
    ParallelNonIntersecting,
    Intersecting,
    NonIntersecting
};

template<typename PrecisionT = float>
class Intersection {
public:
    Point2D<PrecisionT> start;
    Point2D<PrecisionT> end;
    IntersectionType type;

    Intersection(const LineSegment<PrecisionT> &segmentA, const LineSegment<PrecisionT> &segmentB) {
        auto p = segmentA.bottom, q = segmentB.bottom;
        auto r = segmentA.top - p, s = segmentB.top - q;
        if (segmentA.bottom == segmentB.top) {
            type = IntersectionType::Intersecting;
            start = segmentA.bottom;
            return;
        } else if (segmentB.bottom == segmentA.top) {
            type = IntersectionType::Intersecting;
            start = segmentB.bottom;
            return;
        }

        auto diff = q - p;
        auto denom = r.cross(s);
        auto uNumerator = diff.cross(r);
        if (approx_eq(denom, 0)) {
            if (approx_eq(uNumerator, 0)) {
                // lines are collinear
                auto rSq = r.dot(r), sSq = s.dot(s);
                auto t_q = diff.dot(r) / rSq;
                auto u_p = -diff.dot(s) / sSq;
                if (approx_in_range(t_q, 0, 1)) {
                    start = p + r * t_q;
                    auto t_s = t_q + s.dot(r) / rSq;
                    if (approx_in_range(t_s, 0, 1)) {
                        end = p + r * t_s;
                    } else {
                        end = segmentA.top;
                    }
                    type = IntersectionType::CollinearOverlapping;
                } else if (approx_in_range(u_p, 0, 1)) {
                    start = q + s * u_p;
                    auto u_r = u_p + r.dot(s) / sSq;
                    if (approx_in_range(u_r, 0, 1)) {
                        end = q + s * u_r;
                    } else {
                        end = segmentB.top;
                    }
                    type = IntersectionType::CollinearOverlapping;
                } else {
                    type = IntersectionType::CollinearDisjoint;
                }
                return;
            } else {
                type = IntersectionType::ParallelNonIntersecting;
                return;
            }
        } else {
            auto t = diff.cross(s) / denom;
            auto u = uNumerator / denom;
            if (approx_in_range(t, 0, 1) and approx_in_range(u, 0, 1)) {
                start = p + r * t;
                type = IntersectionType::Intersecting;
            } else {
                type = IntersectionType::NonIntersecting;
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Intersection &intersection) {
        switch (intersection.type) {
            case IntersectionType::Intersecting : {
                os << "start: " << intersection.start << " type: intersecting";
                break;
            }
            case IntersectionType::NonIntersecting : {
                os << "type: NonIntersecting";
                break;
            }
            case IntersectionType::ParallelNonIntersecting : {
                os << "type: ParallelNonIntersecting";
                break;
            }
            case IntersectionType::CollinearDisjoint : {
                os << "type: CollinearDisjoint";
                break;
            }
            case IntersectionType::CollinearOverlapping :
                os << "start: " << intersection.start << " end: " << intersection.end << " type: CollinearOverlapping";
                break;
        }
        return os;
    }
};

#endif //INTERSECTION_HPP
