//
// Created by olber on 10/6/2022.
//
#include "RandomNumberGenerator.h"
#include <chrono>

double utility::get_random_number(double low, double high) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<double> dis(low, high);
    return dis(gen);
}