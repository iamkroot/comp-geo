#include <fstream>
#include <list>
#include <map>
#include <set>
#include "UnorderedSet.hpp"
#include "LineSegment.hpp"
#include "Intersection.hpp"

/**
 * @brief Read the input either from a file or stdin
 * @return List of lines
 */
template<typename PrecisionT>
auto readLines(int argc, char* argv[]) {
    std::ifstream file;
    if (argc > 1) {
        file.open(argv[1]);
        if (!file) throw std::runtime_error("Can't open file");
        std::cin.rdbuf(file.rdbuf());
    }
    std::list<LineSegment<PrecisionT>> lines;
    int n;
    std::cin >> n;
    Point2D<PrecisionT> start, end;
    while (n--) {
        std::cin >> start.x >> start.y >> end.x >> end.y;
        lines.emplace_back(start, end);
    }
    return lines;
}

void printSet(auto &s) {
//    for (const auto &item : s) {
//        std::cout << *item << ", ";
//    }
//    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    using PrecisionT = double;
    auto lines = readLines<PrecisionT>(argc, argv);

    using LinePtrs = UnorderedSet<LineSegment<PrecisionT>*>;  /**< set of line segment pointers */
    std::map<Point2D<PrecisionT>, LinePtrs> Q;  /**< Event queue - implicitly stores set U(p) */
    std::map<Point2D<PrecisionT>, LinePtrs> intersections;  /**< Store the intersections */

    // Add endpoints of segments to Q
    for (auto &line : lines) {
        Q[line.top].insert(&line);
        Q[line.bottom];  // add bottom point if it doesn't exist in Q
    }

//    auto inf = std::numeric_limits<PrecisionT>::max();
    auto inf = 10000.0;
//    auto eps = std::numeric_limits<PrecisionT>::epsilon();
    auto eps = 0.00000;
    auto sweepY = inf;
    Point2D curEvent{inf, inf};

    /**
     * @brief Comparator for maintaining Status structure, segments sorted left to right according to abscissa at sweep
     *     line.
     */
    auto cmp = [&sweepY, &eps, &curEvent, &inf](const LineSegment<PrecisionT>* l, const LineSegment<PrecisionT>* r) {
        // horizontal segments should be last among all that contain the current event
        if (l->isHorizontal and approx_eq(l->top.y, sweepY)) {
            if (r->bottom.y == -inf)  // r is dummy segment
                return l->bottom.x < r->bottom.x;
            return not r->contains(curEvent);
        }
        if (r->isHorizontal and approx_eq(r->top.y, sweepY)) {
            if (l->bottom.y == -inf)  // l is dummy segment
                return l->bottom.x < r->bottom.x;
            return l->contains(curEvent);
        }
        auto lx = l->interpX(sweepY - eps), rx = r->interpX(sweepY - eps);  /**< get x coords at current sweep line */
        return approx_eq(lx, rx) ? l->bottom.x < r->bottom.x : lx < rx;
    };

    std::set<LineSegment<PrecisionT>*, decltype(cmp)> T(cmp);  /**< status tree */

    /**
     * @brief Find segments in T that contain the given point
     * @return Set of segments with P as bottom endpoint, or containing P in interior
     */
    auto getContainingSegments = [&T, &inf](const Point2D<PrecisionT> &point) {
        auto leftDummy = new LineSegment<PrecisionT>(point, {-inf, -inf});  /**< dummy to find position of segments */
        auto rightDummy = new LineSegment<PrecisionT>(point, {inf, -inf});  /**< dummy to find position of segments */

        auto[leftPos, _] = T.insert(leftDummy);
        auto[rightPos, __] = T.insert(rightDummy);
        printSet(T);
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
        auto dummy = new LineSegment<PrecisionT>(point, {point.x, -inf});

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
            std::cout << "INT: " << intersection.start << std::endl;
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
        curEvent = p;
        auto U = node.mapped();  /**< Set of line segments with p as upper endpoint */
        printSet(T);
        sweepY = p.y;
        auto segments = getContainingSegments(p);
        auto LuCuU = segments | U;
        if (LuCuU.size() > 1) {  // report the intersections
            intersections[p] = LuCuU;
            std::cout << "Intersections: ";
            printSet(LuCuU);

        }

        // Remove L u C from T
        for (auto &segment : segments) {
            T.erase(segment);
        }
        printSet(T);

        bool eventLower = true;  /**< Event point is only the lower endpoint of one or more lines */

        decltype(T)::iterator leftMostPos = T.end(), rightMostPos = T.end();  /**< Store the leftmost segment from CuU in T */
        PrecisionT minX = inf, maxX = -inf;
//        decltype(T)::iterator leftMostPos = T.end();  /**< Store the leftmost segment from CuU in T */
//        decltype(T)::reverse_iterator rightMostPos = T.rend();  /**< Store the rightmost segment from CuU in T */
//        unsigned long minLeftDist = T.size(), minRightDist = T.size();

        // insert segments from C or U into T
        for (auto &segment : LuCuU) {
            if (segment->bottom == p)  // segment is from L
                continue;
            auto[pos, _] = T.insert(segment);
            auto x = segment->interpX(sweepY - eps);
            if (x < minX) {
                x = minX;
                leftMostPos = pos;
            }
            if (x > maxX) {
                x = maxX;
                rightMostPos = pos;
            }
//            auto leftDist = std::distance(T.begin(), pos), rightDist = std::distance(pos, T.end());
//            if (leftDist <= minLeftDist) {
//                leftMostPos = pos;
//                minLeftDist = leftDist;
//            }
//            if (rightDist <= minRightDist) {
//                rightMostPos = std::make_reverse_iterator(std::next(pos));
//                minRightDist = rightDist;
//            }
            eventLower = false;  // point is either an upper end point or in interior of a segment
        }
        printSet(T);

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
//            if (rightMostPos != T.rend() and rightMostPos != T.rbegin()) {
//                auto rightPos = std::prev(rightMostPos);  // rightMostPos is a reverse iterator
//                findNewEvent(**rightMostPos, **rightPos, p);
//            }
            if (rightMostPos != T.end()) {
                auto rightPos = std::next(rightMostPos);
                if (rightPos != T.end())
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