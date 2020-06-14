#include <list>
#include <map>
#include <set>
#include "LineSegment.hpp"

int main() {
    using LinePtrs = std::list<LineSegment<float>*>; /**< list of line segment pointers */
    std::map<Point2D<float>, LinePtrs> Q;  /**< implicitly stores set U(p) */
    std::list<LineSegment<float>> lines = {
            // TODO: Read lines
            {{1, 1}, {2, 2}},
            {{2, 2}, {4, 4}},
    };

    // Add endpoints of segments to Q
    for (auto &line : lines) {
        Q[line.top].push_back(&line);
        Q[line.bottom];  // add bottom point if it doesn't exist in Q
    }

    float sweepY = std::numeric_limits<float>::infinity();
    auto cmp = [&sweepY](const LineSegment<float>* l, const LineSegment<float>* r) {
        auto lx = l->interpX(sweepY), rx = r->interpX(sweepY);  /**< get x coords at current sweep line */
        return lx < rx;
    };

    std::set<LineSegment<float>*, decltype(cmp)> T(cmp);  /**< status tree */

    while (!Q.empty()) {
        auto node = Q.extract(Q.begin());
        auto p = node.key();  /**< Event point */
        auto U = node.mapped();  /**< Set of line segments with p as upper endpoint */

        sweepY = p.y;
    }
    return 0;
}