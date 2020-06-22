#include <vector>
#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <tuple>
#include "Stopwatch.hpp"

/**
 * @brief Represents a point in 2 dimensional space
 */
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

/**
 * @brief Run the greedy 2-approximation algorithm to find k-clustering of points
 * @returns The selected centers, which center each point belongs to, and it's distance from that center
 */
auto greedyKCenter(const std::vector<Point2D> &points, int k) {
    std::vector<Point2D> centers;
    std::vector<int> centerLabels(points.size());
    std::vector<float> dist(points.size(), std::numeric_limits<float>::max());
    for (int i = 0; i < k; ++i) {
        auto center_ind = std::distance(dist.begin(), std::max_element(dist.begin(), dist.end()));
        auto center = points[center_ind];
        centers.push_back(center);
        for (int j = 0; j < points.size(); ++j) {
            auto d = center.dist_sq(points[j]);
            if (d < dist[j]) {
                dist[j] = d;
                centerLabels[j] = i;
            }
        }
    }
    return std::make_tuple(centers, centerLabels, dist);
}

/**
 * @brief Read the points and num_centers from a file or cin
 */
auto readDataset(std::istream* file) {
    std::vector<Point2D> points;
    int k, n;
    (*file) >> n >> k;
    while (n--) {
        Point2D point(0, 0);
        (*file) >> point.x >> point.y;
        points.push_back(point);
    }
    return std::make_pair(points, k);
}

int main(int argc, char* argv[]) {
    std::istream* input;
    std::ifstream file;
    if (argc < 2) {
        input = &std::cin;
    } else {
        file.open(argv[1]);
        input = &file;
    }
    auto[points, k] = readDataset(input);
    Stopwatch stopwatch;
    auto[centers, labels, dist] = greedyKCenter(points, k);
    auto runtime = stopwatch.stop();

    for (const auto &c : centers) {
        std::cout << c << std::endl;
    }

    for (const auto &label : labels) {
        std::cout << label << std::endl;
    }

    for (const auto &d : dist) {
        std::cout << d << std::endl;
    }

    std::cout<<runtime<<std::endl;
    return 0;
}
