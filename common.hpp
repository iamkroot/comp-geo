#ifndef COMMON_HPP
#define COMMON_HPP

#include <limits>
#include <cmath>

/**
 * @brief Test if the given arguments are approximately equal
 * @return True if the args are within the minimum range after which precision lo
 */
template <typename PrecisionT = float>
static inline bool approx_eq(PrecisionT x, PrecisionT y) {
    if (x == y)
        return true;
    return std::abs(x - y) < std::numeric_limits<PrecisionT>::epsilon();
}

#endif //COMMON_HPP
