//
// Created by olber on 9/16/2022.
//
#pragma once

#include <random>

namespace utility {
    double get_random_number(double low, double high) {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(low, high);
        return dis(gen);
    }
}