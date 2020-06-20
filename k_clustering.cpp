#include <vector>
#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <tuple>

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
    auto[centers, labels, dist] = greedyKCenter(points, k);

    for (const auto &c : centers) {
        std::cout << c << std::endl;
    }

    for (const auto &label : labels) {
        std::cout << label << std::endl;
    }

    for (const auto &d : dist) {
        std::cout << d << std::endl;
    }
    return 0;
}
