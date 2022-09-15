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

namespace linear_algebra_core
{
    template <size_t N>
    class Vector_X
    {
    private:
        std::array<double, N> m_values;

    public:
        Vector_X() : m_values{} { }

        template <typename... InitialValues>
        Vector_X(InitialValues... initialValues)
        {
            static_assert(N == sizeof...(InitialValues), "Incorrect number of parameters given to constructor");
            static_assert((std::is_convertible_v<InitialValues, double> && ...), "Parameters must be implicitly convertible to doubles");
            m_values = std::array<double, N>{initialValues...};
        }

        explicit Vector_X(const std::array<double, N>& initialValues) : m_values{initialValues} { }
        explicit Vector_X(const std::array<double, N>&& initialValues) : m_values{initialValues} { }
        explicit Vector_X(const double (&initialValues)[N]) {
            std::copy_n(std::begin(initialValues), N, std::begin(m_values));
        }
        explicit Vector_X(const std::vector<double>& initialValues) {
            if(initialValues.size() != N) {
                throw std::out_of_range("Vector_X constructor expected a std::vector of size " + std::to_string(N) + ", but got size " + std::to_string(initialValues.size()));
            }
            std::copy_n(std::begin(initialValues), N, std::begin(m_values));
        }

        ~Vector_X() = default;
        Vector_X(const Vector_X<N>& other) = default;
        Vector_X(Vector_X<N>&& other) noexcept = default;
        Vector_X<N>& operator=(const Vector_X<N>& other) = default;
        Vector_X<N>& operator=(Vector_X<N>&& other) noexcept = default;

        // iterator exposure
        auto begin()   { return std::begin(m_values);   }
        auto end()     { return std::end(m_values);     }
        auto rbegin()  { return std::rbegin(m_values);  }
        auto rend()    { return std::rend(m_values);    }
        auto cbegin()  const { return std::cbegin(m_values);  }
        auto cend()    const { return std::cend(m_values);    }
        auto crbegin() const { return std::crbegin(m_values); }
        auto crend()   const { return std::crend(m_values);   }

        [[nodiscard]] inline double dot(const Vector_X<N>& rhs) const
        {
            return (*this) * rhs;
        }

        Vector_X<N> cross(const Vector_X<N>& rhs) const
        {
            static_assert(N == 3, "cross product can only be computed on 3 dimensional vectors");
            return {
                m_values[1] * rhs[2] - m_values[2] * rhs[1],
                m_values[2] * rhs[0] - m_values[0] * rhs[2],
                m_values[0] * rhs[1] - m_values[1] * rhs[0]
            };
        }

        Vector_X<N>& fill(double value)
        {
            std::fill(begin(), end(), value);
            return (*this);
        }

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

        [[nodiscard]] double operator*(const Vector_X<N>& rhs) const
        {
            double result = 0;
            for(int i = 0; i < N; i++) {
                result += m_values[i] * rhs[i];
            }
            return result;
        }

        template<typename T>
        [[nodiscard]] Vector_X<N> operator*(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            Vector_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value * scalar; });
            return result;
        }

        template<typename T>
        [[nodiscard]] friend inline Vector_X<N> operator*(T scalar, const Vector_X<N>& rhs)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return rhs * scalar;
        }

        template<typename T>
        Vector_X<N>& operator*=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value * scalar; });
            return *this;
        }

        template<typename T>
        [[nodiscard]] Vector_X<N> operator/(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            if(scalar == 0.0)
            {
                throw std::domain_error("Cannot divide a vector by 0");
            }
            Vector_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value / scalar; });
            return result;
        }

        template<typename T>
        Vector_X<N>& operator/=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            if(scalar == 0.0)
            {
                throw std::domain_error("Cannot divide a vector by 0");
            }
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value / scalar; });
            return *this;
        }

        [[nodiscard]] Vector_X<N> operator+(const Vector_X<N>& rhs) const
        {
            Vector_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left + right; });
            return result;
        }

        Vector_X<N>& operator+=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left + right; });
            return *this;
        }

        [[nodiscard]] inline Vector_X<N> operator-() const
        {
            return (*this) * -1.0;
        }

        [[nodiscard]] Vector_X<N> operator-(const Vector_X<N>& rhs) const
        {
            Vector_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left - right; });
            return result;
        }

        Vector_X<N>& operator-=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left - right; });
            return *this;
        }

        [[nodiscard]] bool operator==(const Vector_X<N>& rhs) const
        {
            bool isEqual = true;
            for(int i = 0; isEqual && i < N; i++) {
                isEqual = isEqual && (m_values[i] == rhs[i]);
            }
            return isEqual;
        }

        [[nodiscard]] inline bool operator!=(const Vector_X<N>& rhs) const
        {
            return !(*this == rhs);
        }

        friend std::ostream& operator<<(std::ostream& out, const Vector_X<N>& rhs)
        {
            std::string output = "{ ";
            for(int i = 0; i < (N - 1); i++) {
                output += std::to_string(rhs[i]) + ", ";
            }
            if(N > 0) {
                output += std::to_string(rhs[N - 1]);
            }
            output += " }";
            out << output;
            return out;
        }

        [[nodiscard]] inline double getMagnitudeSquared() const
        {
            double result = 0;
            for(int i = 0; i < N; i++) {
                result += m_values[i] * m_values[i];
            }
            return result;
        }

        [[nodiscard]] inline double getMagnitude() const
        {
            return std::sqrt(getMagnitudeSquared());
        }

        inline Vector_X<N>& normalize()
        {
            (*this) /= getMagnitude();
            return *this;
        }

        [[nodiscard]] inline Vector_X<N> getUnitVector() const
        {
            return (*this) / getMagnitude();
        }

        Vector_X<N>& square()
        {
            std::transform(cbegin(), cend(), begin(), [](auto value) { return value * value; });
            return (*this);
        }

        [[nodiscard]] inline Vector_X<N> getSquared() const
        {
            Vector_X<N> result {*this};
            return result.square();
        }

        [[nodiscard]] inline double sum_elements() const
        {
            return std::accumulate(cbegin(), cend(), 0);
        }

        template<size_t M>
        Vector_X<M> getAsDimension() const
        {
            Vector_X<M> result;
            for(int i = 0; i < M; i++) {
                if(i < N) {
                    result[i] = m_values[i];
                } else {
                    result[i] = 0.0;
                }
            }
            return result;
        }

        [[nodiscard]] static inline double triple_scalar_product(const Vector_X<N>& a, const Vector_X<N>& b, const Vector_X<N>& c)
        {
            static_assert(N == 3, "triple scalar product can only be computed on 3 dimensional vectors");
            return a * b.cross(c);
        }

        [[nodiscard]] static Vector_X<N> linear_interpolation(const Vector_X<N>& a, const Vector_X<N>& b, double t)
        {
            if(t > 1.0 || t < 0.0)
            {
                throw std::invalid_argument("t must be in the range of [0.0, 1.0]. Given value was: " + std::to_string(t));
            }
            return (a * t) + (b * (1.0 - t));
        }
    };

    using Vector_3 = Vector_X<3>;
    using Vector_4 = Vector_X<4>;
}