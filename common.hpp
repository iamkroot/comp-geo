#ifndef COMMON_HPP
#define COMMON_HPP

#include <limits>
#include <cmath>

/**
 * @brief Test if the given arguments are approximately equal
 * @return True if the args are within the minimum range after which precision lo
 */
template<typename PrecisionT1 = float, typename PrecisionT2 = float>
static inline bool approx_eq(PrecisionT1 x, PrecisionT2 y) {
    if (x == y)
        return true;
    return std::abs(x - y) < std::numeric_limits<PrecisionT1>::epsilon();
}

template<typename PrecisionT1 = float, typename PrecisionT2 = float>
static inline bool approx_in_range(PrecisionT1 x, PrecisionT2 start, PrecisionT2 end) {
    const auto eps = std::numeric_limits<PrecisionT1>::epsilon();
    return start - eps <= x and x <= end + eps;
}

#endif //COMMON_HPP
