#include "LineSegment.hpp"

int main() {
    auto l = LineSegment<float>({3, 3}, {5, 3});
    std::cout<<l.top<<l.bottom<<std::endl;
    std::cout<<l.contains({3,4})<<std::endl;
    std::cout<<l.contains({4,3})<<std::endl;
    std::cout<<l.contains({3,6})<<std::endl;
}