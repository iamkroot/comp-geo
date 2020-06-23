#ifndef UNORDEREDSET_H
#define UNORDEREDSET_H

#include <unordered_set>

/**
 * @brief Extension of std::unordered_set with overridden operators for set operations
 */
template<typename T>
class UnorderedSet : public std::unordered_set<T> {
public:
    /**
     * @brief Intersection operator
     * @param other
     * @return A set contain elements present in both sets
     */
    UnorderedSet<T> operator&(const UnorderedSet &other) {
        UnorderedSet<T> intersection;
        for (auto item : *this) {
            if (other.contains(item))
                intersection.insert(item);
        }
        return intersection;
    }

    /**
     * @brief Union operator
     * @param other
     * @return A set containing elements present in lhs or rhs or both
     */
    UnorderedSet<T> operator|(const UnorderedSet &other) {
        UnorderedSet<T> union_;
        union_.insert(this->begin(), this->end());
        union_.insert(other.begin(), other.end());
        return union_;
    }

    /**
     * @brief Difference operator
     * @param other
     * @return A set containing all elements of lhs that are not present in rhs
     */
    UnorderedSet<T> operator-(const UnorderedSet &other) {
        UnorderedSet<T> diff;
        for (auto item : *this) {
            if (not other.contains(item))
                diff.insert(item);
        }
        return diff;
    }
};

#endif //UNORDEREDSET_H
