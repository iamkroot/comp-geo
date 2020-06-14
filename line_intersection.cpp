#include <map>
#include <list>
#include "LineSegment.hpp"

int main() {
    using LinePtrs = std::list<LineSegment<float>*>; /**< list of line segment pointers */
    std::map<Point2D<float>, LinePtrs> Q;  /**< implicitly stores set U(p) */
    std::list<LineSegment<float>> lines = {
            // TODO: Read lines
    };

    // Add endpoints of segments to Q
    for (auto &line : lines) {
        Q[line.top].push_back(&line);
        Q[line.bottom];  // add bottom point if it doesn't exist in Q
    }

    while (!Q.empty()) {
        auto node = Q.extract(Q.begin());
        auto p = node.key();  /**< Event point */
        auto U = node.mapped();  /**< Set of line segments with p as upper endpoint */

    }
}