//
// Created by olber on 8/10/2022.
//

#pragma once
#include <cstdlib>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <vector>
#include <numeric>

#include "Vector_X.h"

namespace linear_algebra_core
{
    template <size_t N>
    class Point_X
    {
    private:
        std::array<double, N> m_values;

    public:
        Point_X() : m_values{} { }

        template <typename... InitialValues>
        Point_X(InitialValues... initialValues)
        {
            static_assert((std::is_convertible_v<InitialValues, double> && ...), "Parameters must be implicitly convertible to doubles");
            static_assert(N == sizeof...(InitialValues), "Incorrect number of parameters given to constructor");
            m_values = std::array<double, N>{initialValues...};
        }

        explicit Point_X(const std::array<double, N>& initialValues) : m_values{initialValues} { }
        explicit Point_X(const std::array<double, N>&& initialValues) : m_values{initialValues} { }
        explicit Point_X(const double (&initialValues)[N]) {
            std::copy_n(std::begin(initialValues), N, std::begin(m_values));
        }
        explicit Point_X(const std::vector<double>& initialValues) {
            if(initialValues.size() != N) {
                throw std::out_of_range("Point_X constructor expected a std::vector of size " + std::to_string(N) + ", but got size " + std::to_string(initialValues.size()));
            }
            std::copy_n(std::begin(initialValues), N, std::begin(m_values));
        }

        ~Point_X() = default;
        Point_X(const Point_X<N>& other) = default;
        Point_X(Point_X<N>&& other) noexcept = default;
        Point_X<N>& operator=(const Point_X<N>& other) = default;
        Point_X<N>& operator=(Point_X<N>&& other) noexcept = default;

        // iterator exposure
        auto begin()   { return std::begin(m_values);   }
        auto end()     { return std::end(m_values);     }
        auto rbegin()  { return std::rbegin(m_values);  }
        auto rend()    { return std::rend(m_values);    }
        auto cbegin()  const { return std::cbegin(m_values);  }
        auto cend()    const { return std::cend(m_values);    }
        auto crbegin() const { return std::crbegin(m_values); }
        auto crend()   const { return std::crend(m_values);   }

        [[nodiscard]] inline double operator[](size_t index) const
        {
            return m_values[index];
        }

        [[nodiscard]] inline double& operator[](size_t index)
        {
            return m_values[index];
        }

        [[nodiscard]] inline double at(size_t index) const
        {
            return m_values.at(index);
        }

        [[nodiscard]] inline double& at(size_t index)
        {
            return m_values.at(index);
        }

        template<typename T>
        [[nodiscard]] Point_X<N> operator*(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            Point_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value * scalar; });
            return result;
        }

        template<typename T>
        [[nodiscard]] friend inline Point_X<N> operator*(T scalar, const Point_X<N>& rhs)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return rhs * scalar;
        }

        template<typename T>
        Point_X<N>& operator*=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value * scalar; });
            return *this;
        }

        template<typename T>
        [[nodiscard]] Point_X<N> operator/(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            if(scalar == 0.0)
            {
                throw std::domain_error("Cannot divide a vector by 0");
            }
            Point_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value / scalar; });
            return result;
        }

        template<typename T>
        Point_X<N>& operator/=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            if(scalar == 0.0)
            {
                throw std::domain_error("Cannot divide a vector by 0");
            }
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value / scalar; });
            return *this;
        }

        [[nodiscard]] Point_X<N> operator+(const Vector_X<N>& rhs) const
        {
            Point_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left + right; });
            return result;
        }

        Point_X<N>& operator+=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left + right; });
            return *this;
        }

        [[nodiscard]] inline Point_X<N> operator-() const
        {
            return (*this) * -1.0;
        }

        [[nodiscard]] Vector_X<N> operator-(const Point_X<N>& rhs) const
        {
            Vector_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left - right; });
            return result;
        }

        [[nodiscard]] Point_X<N> operator-(const Vector_X<N>& rhs) const
        {
            Point_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left - right; });
            return result;
        }

        Point_X<N>& operator-=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left - right; });
            return *this;
        }

        [[nodiscard]] bool operator==(const Point_X<N>& rhs) const
        {
            bool isEqual = true;
            for(int i = 0; isEqual && i < N; i++) {
                isEqual = isEqual && (m_values[i] == rhs[i]);
            }
            return isEqual;
        }

        [[nodiscard]] inline bool operator!=(const Point_X<N>& rhs) const
        {
            return !(*this == rhs);
        }

        friend std::ostream& operator<<(std::ostream& out, const Point_X<N>& rhs)
        {
            std::string output = "{ ";
            for(int i = 0; i < (N - 1); i++) {
                output += std::to_string(rhs[i]) + ", ";
            }
            if(N > 0)
            {
                output += std::to_string(rhs[N - 1]) + " }";
            }
            out << output;
            return out;
        }

        Point_X<N>& square()
        {
            std::transform(cbegin(), cend(), begin(), [](auto value) { return value * value; });
            return *this;
        }

        [[nodiscard]] inline Point_X<N> getSquared() const
        {
            Point_X<N> result {*this};
            return result.square();
        }

        [[nodiscard]] inline double sum_elements() const
        {
            return std::accumulate(cbegin(), cend(), 0);
        }

        template<size_t M>
        Point_X<M> getAsDimension() const
        {
            Point_X<M> result;
            for(int i = 0; i < M; i++) {
                if(i < N) {
                    result[i] = m_values[i];
                } else {
                    result[i] = 0.0;
                }
            }
            return result;
        }

        [[nodiscard]] inline Vector_X<N> to_Vector() const
        {
            return Vector_X<N>{m_values};
        }

        [[nodiscard]] static Point_X<N> linear_interpolation(const Point_X<N>& a, const Point_X<N>& b, double t)
        {
            if(t > 1.0 || t < 0.0)
            {
                throw std::invalid_argument("t must be in the range of [0.0, 1.0]. Given value was: " + std::to_string(t));
            }
            return (a * t) + (b * (1.0 - t));
        }
    };

    using Point_3 = Point_X<3>;
    using Point_4 = Point_X<4>;
}