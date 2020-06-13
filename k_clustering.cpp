#include <vector>
#include <iostream>
#include <limits>
#include <cmath>

struct Point2D {
    float x, y;

    Point2D(float x, float y) : x(x), y(y) {}

    float dist_sq(Point2D other) {
        float dx = x - other.x, dy = y - other.y;
        return dx * dx + dy * dy;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point2D &d) {
        os << "(" << d.x << ", " << d.y << ")";
        return os;
    }
};

std::pair<std::vector<Point2D>, float> greedyKCenter(const std::vector<Point2D> &points, int k) {
    float max_dist = std::numeric_limits<float>::max();
    std::vector<Point2D> centers;
    std::vector<float> dist(points.size(), std::numeric_limits<float>::max());
    for (int i = 0; i < k; ++i) {
        auto center_ind = std::distance(dist.begin(), std::max_element(dist.begin(), dist.end()));
        auto center = points[center_ind];
        centers.push_back(center);
        for (int j = 0; j < points.size(); ++j) {
            auto d = center.dist_sq(points[j]);
            dist[j] = std::min(dist[j], d);
        }
    }
    return {centers, std::sqrt(*std::max_element(dist.begin(), dist.end()))};
}

int main() {
    std::vector<Point2D> points = {};
    auto[cent, d] = greedyKCenter(points, 0);
    std::cout << d << std::endl;
    for (const auto &c : cent) {
        std::cout << c << ", ";
    }
    return 0;
}
