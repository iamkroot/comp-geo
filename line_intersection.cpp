#include <list>
#include <map>
#include <set>
#include "UnorderedSet.hpp"
#include "LineSegment.hpp"
#include "Intersection.hpp"

int main() {
    using PrecisionT = float;
    using LinePtrs = UnorderedSet<LineSegment<PrecisionT>*>;  /**< list of line segment pointers */
    std::map<Point2D<PrecisionT>, LinePtrs> Q;  /**< Event queue - implicitly stores set U(p) */
    std::list<LineSegment<PrecisionT>> lines = {
            // TODO: Read lines
            {{1, 1}, {2, 2}},
            {{2, 2}, {4, 4}},
            {{1, 3}, {3, 1}},
            {{4, 4}, {3, 2}},
            {{1, 4}, {0, 1}},
    };

    std::map<Point2D<PrecisionT>, LinePtrs> intersections;  /**< Store the intersections */

    // Add endpoints of segments to Q
    for (auto &line : lines) {
        Q[line.top].insert(&line);
        Q[line.bottom];  // add bottom point if it doesn't exist in Q
    }

    auto inf = std::numeric_limits<PrecisionT>::max();
    auto sweepY = inf;

    /**
     * @brief Comparator for maintaining Status structure, segments sorted left to right according to abscissa at sweep
     *     line.
     */
    auto cmp = [&sweepY](const LineSegment<PrecisionT>* l, const LineSegment<PrecisionT>* r) {
        auto lx = l->interpX(sweepY), rx = r->interpX(sweepY);  /**< get x coords at current sweep line */
        return approx_eq(lx, rx) ? l->top.x < r->top.x : lx < rx;
    };

    std::set<LineSegment<PrecisionT>*, decltype(cmp)> T(cmp);  /**< status tree */

    /**
     * @brief Find segments in T that contain the given point
     * @return Set of segments with P as bottom endpoint, or containing P in interior
     */
    auto getContainingSegments = [&T, &inf](const Point2D<PrecisionT> &point) {
        auto leftDummy = new LineSegment<PrecisionT>(point, {-inf, inf});  /**< dummy to find position of segments */
        auto rightDummy = new LineSegment<PrecisionT>(point, {inf, inf});  /**< dummy to find position of segments */

        auto[leftPos, _] = T.insert(leftDummy);
        auto[rightPos, __] = T.insert(rightDummy);

        LinePtrs segments;

        for (auto it = leftPos; it != T.end() and it != rightPos; it++) {
            if (it == leftPos)
                continue;
            segments.insert(*it);
        }
        T.erase(leftDummy);
        T.erase(rightDummy);

        delete leftDummy;
        delete rightDummy;

        return segments;
    };

    auto getNeighbors = [&T, &inf](const Point2D<PrecisionT> &point) {
        auto dummy = new LineSegment<PrecisionT>(point, {point.x, inf});

        auto[pos, _] = T.insert(dummy);
        auto leftIter = std::prev(pos), rightIter = std::next(pos);
        LineSegment<PrecisionT>* left = nullptr, * right = nullptr;

        if (pos != T.begin() and rightIter != T.end()) {
            left = *leftIter;
            right = *rightIter;
        }
        T.erase(dummy);
        delete dummy;
        return std::make_pair(left, right);
    };

    auto findNewEvent = [&Q, &sweepY](LineSegment<PrecisionT> &leftSeg, LineSegment<PrecisionT> &rightSeg,
                                      Point2D<PrecisionT> &point) {
        auto intersection = Intersection(leftSeg, rightSeg);
        if (intersection.type == IntersectionType::Intersecting or
            intersection.type == IntersectionType::CollinearOverlapping) {
            if ((intersection.start.y < sweepY or
                 (approx_eq(intersection.start.y, sweepY) and intersection.start.x > point.x)) and
                not Q.contains(intersection.start)) {
                Q[intersection.start];
            }
        }
    };

    while (!Q.empty()) {
        auto node = Q.extract(Q.begin());
        auto p = node.key();  /**< Event point */
        auto U = node.mapped();  /**< Set of line segments with p as upper endpoint */

        sweepY = p.y;
        auto segments = getContainingSegments(p);
        auto LuCuU = segments | U;
        if (LuCuU.size() > 1) {  // report the intersections
            intersections[p] = LuCuU;
        }

        // Remove L u C from T
        for (auto &segment : segments) {
            T.erase(segment);
        }

        bool eventLower = true;  /**< Event point is only the lower endpoint of one or more lines */

        decltype(T)::iterator leftMostPos = T.end();  /**< Store the leftmost segment from CuU in T */
        decltype(T)::reverse_iterator rightMostPos = T.rend();  /**< Store the rightmost segment from CuU in T */
        unsigned long minLeftDist = T.size(), minRightDist = T.size();

        // insert segments from C or U into T
        for (auto &segment : LuCuU) {
            if (segment->bottom == p)  // segment is from L
                continue;
            auto[pos, _] = T.insert(segment);
            auto leftDist = std::distance(T.begin(), pos), rightDist = std::distance(pos, T.end());
            if (leftDist <= minLeftDist) {
                leftMostPos = pos;
                minLeftDist = leftDist;
            }
            if (rightDist <= minRightDist) {
                rightMostPos = std::make_reverse_iterator(pos);
                minRightDist = rightDist;
            }
            eventLower = false;  // point is either an upper end point or in interior of a segment
        }
        if (eventLower) {  // two segments become neighbors again
            auto[left, right] = getNeighbors(p);
            if (left and right) {
                findNewEvent(*left, *right, p);
            }
        } else {
            if (leftMostPos != T.end() and leftMostPos != T.begin()) {
                auto leftPos = std::prev(leftMostPos);
                findNewEvent(**leftPos, **leftMostPos, p);
            }
            if (rightMostPos != T.rend() and rightMostPos != T.rbegin()) {
                auto rightPos = std::prev(rightMostPos);  // rightMostPos is a reverse iterator
                findNewEvent(**rightMostPos, **rightPos, p);
            }
        }
    }

    for (auto &intersection : intersections) {
        std::cout << "Intersecting point: " << intersection.first << std::endl << "Segments:" << std::endl;
        for (const auto &segment : intersection.second) {
            std::cout << *segment << std::endl;
        }
    }
    return 0;
}