#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include "LineSegment.hpp"

int main() {
    using PrecisionT = float;
    using LinePtrs = std::unordered_set<LineSegment<PrecisionT>*>;  /**< list of line segment pointers */
    std::map<Point2D<PrecisionT>, LinePtrs> Q;  /**< Event queue - implicitly stores set U(p) */
    std::list<LineSegment<PrecisionT>> lines = {
            // TODO: Read lines
//            {{1, 1}, {2, 2}},
            {{2, 2}, {4, 4}},
            {{1, 3}, {3, 1}},
            {{4, 4}, {3, 2}},
            {{1, 4}, {0, 1}},
    };

    // Add endpoints of segments to Q
    for (auto &line : lines) {
        Q[line.top].insert(&line);
        Q[line.bottom];  // add bottom point if it doesn't exist in Q
    }

    auto inf = std::numeric_limits<PrecisionT>::max();
    auto sweepY = inf;

    auto cmp = [&sweepY](const LineSegment<PrecisionT>* l, const LineSegment<PrecisionT>* r) {
        auto lx = l->interpX(sweepY), rx = r->interpX(sweepY);  /**< get x coords at current sweep line */
        return approx_eq(lx, rx) ? l->top.x < r->top.x : lx < rx;
    };

    std::set<LineSegment<PrecisionT>*, decltype(cmp)> T(cmp);  /**< status tree */

    auto getContainingSegments = [&T, &inf](const Point2D<PrecisionT> &point) {
        auto leftDummy = new LineSegment<PrecisionT>(point, {-inf, inf});  /**< dummy to find position of segments */
        auto rightDummy = new LineSegment<PrecisionT>(point, {inf, inf});  /**< dummy to find position of segments */

        auto[leftPos, _] = T.insert(leftDummy);
        auto[rightPos, __] = T.insert(rightDummy);

        std::unordered_set<LineSegment<PrecisionT>*> L, C;

        for (auto it = leftPos; it != T.end() and it != rightPos; it++) {
            if (it == leftPos)
                continue;
            auto line = *it;
            if (line->bottom == point) {
                L.insert(line);
            } else {
                C.insert(line);
            }
        }
        T.erase(leftDummy);
        T.erase(rightDummy);

        return std::make_pair(L, C);
    };

    for (auto &line : lines) {
        T.insert(&line);
    }
    auto [L, C] = getContainingSegments({2, 2});
    for (const auto &line : L) {
        std::cout<<*line<<", ";
    }
    std::cout<<std::endl;
    for (const auto &line : C) {
        std::cout<<*line<<", ";
    }
    std::cout<<std::endl;
    return 0;
    while (!Q.empty()) {
        auto node = Q.extract(Q.begin());
        auto p = node.key();  /**< Event point */
        auto U = node.mapped();  /**< Set of line segments with p as upper endpoint */

        sweepY = p.y;
        std::cout << p << std::endl;
        getContainingSegments(p);
    }
    return 0;
}