//
// Created by olber on 9/16/2022.
//
#pragma once

#include <memory>
#include <random>

#include "LinearAlgebraTypeTraits.h"

namespace utility {
    namespace {
        static __uint128_t randomizer_state;
    }
    void initialize_randomizer(uint64_t initial_state)
    {
        randomizer_state = initial_state;
    }

    template<linear_algebra_core::IsFloatingPoint value_type>
    value_type get_random_number(value_type low, value_type high)
    {
        uint64_t temp;
        {
            randomizer_state *= 0xda942042e4dd58b5;
            temp = randomizer_state >> 64;
        }
        value_type t = static_cast<value_type>(temp) / static_cast<value_type>(std::numeric_limits<uint64_t>::max());
        return low + ((high - low) * t);
    }
}