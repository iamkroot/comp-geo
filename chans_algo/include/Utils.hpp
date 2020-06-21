#pragma once
#include <bits/stdc++.h>

class Utils
{
    /**
     * @brief Private constructor to avoid instantiation
     * 
     */
    Utils() {}

public:
    /**
     * @brief Define Infinity
     */
    static const int INF = 1e9;

    /**
     * @brief Utility to compute square
     * 
     * @param x 
     * @return int 
     */
    static int square(int x)
    {
        return x * x;
    }
};