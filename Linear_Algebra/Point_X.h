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
        explicit Point_X(InitialValues... initialValues)
        {
            static_assert((std::is_convertible_v<InitialValues, double> && ...), "Parameters must be implicitly convertible to doubles");
            static_assert(N == sizeof...(InitialValues), "Incorrect number of parameters given to constructor");
            m_values = std::array<double, N>{initialValues...};
        }

        constexpr Point_X(std::initializer_list<double> initialValues)
        {
            if(initialValues.size() != N) {
                throw std::invalid_argument("Point_X constructor expected a initializer_list of size " + std::to_string(N) + ", but got size " + std::to_string(initialValues.size()));
            }
            std::copy_n(std::begin(initialValues), N, begin());
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
        [[nodiscard]] auto cbegin()  const { return std::cbegin(m_values);  }
        [[nodiscard]] auto cend()    const { return std::cend(m_values);    }
        [[nodiscard]] auto crbegin() const { return std::crbegin(m_values); }
        [[nodiscard]] auto crend()   const { return std::crend(m_values);   }

        /*!
         * @param index index of the value to return
         * @return the value at the given \p index
         */
        [[nodiscard]] inline double operator[](size_t index) const { return m_values[index]; }
        /*!
         * @param index index of the value to return
         * @return the value at the given \p index
         */
        [[nodiscard]] inline double& operator[](size_t index) { return m_values[index]; }

        /*!
         * Throws an exception if index is out of bounds
         * @param index index of the value to return
         * @return the value at the given \p index
         */
        [[nodiscard]] inline double at(size_t index) const { return m_values.at(index); }
        /*!
         * Throws an exception if index is out of bounds
         * @param index index of the value to return
         * @return the value at the given \p index
         */
        [[nodiscard]] inline double& at(size_t index) { return m_values.at(index); }

        /*!
         * Compile-time element access
         * @tparam Index index of the desired value
         * @return a copy of the value at the given \p Index
         */
        template<size_t Index>
        [[nodiscard]] constexpr double getValue() const
        {
            static_assert(Index < N, "index was out of bounds");
            return std::get<Index>(m_values);
        }
        /*!
         * Compile-time element access
         * @tparam Index index of the desired value
         * @return a reference to the value at the given \p Index
         */
        template<size_t Index>
        [[nodiscard]] constexpr double& getValue()
        {
            static_assert(Index < N, "index was out of bounds");
            return std::get<Index>(m_values);
        }

        /*!
         * Scales this Point by \p scalar
         * @tparam T The type of \p scalar. Must be an arithmetic type.
         * @param scalar Value to scale the point by
         * @return The scaled point
         */
        template<typename T>
        [[nodiscard]] Point_X<N> operator*(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            Point_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value * scalar; });
            return result;
        }

        /*!
         * Scales \p rhs by \p scalar
         * @tparam T Type of \p scalar. Must be an arithmetic type
         * @param scalar value to scale \p rhs by
         * @param rhs Point to be scaled
         * @return the scaled point
         */
        template<typename T>
        [[nodiscard]] friend inline Point_X<N> operator*(T scalar, const Point_X<N>& rhs)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return rhs * scalar;
        }

        /*!
         * Scale this point by \p scalar
         * @tparam T Type of \p scalar. Must be an arithmetic type
         * @param scalar value to scale this Point by
         * @return a reference to this Point
         */
        template<typename T>
        Point_X<N>& operator*=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value * scalar; });
            return *this;
        }

        /*!
         * Scale this point by 1/\p scalar
         * @tparam T Type of \p scalar. Must be an arithmetic type
         * @param scalar value to scale this point by
         * @return the scaled point
         */
        template<typename T>
        [[nodiscard]] Point_X<N> operator/(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            if(scalar == 0.0)
            {
                throw std::domain_error("Cannot divide a point by 0");
            }
            Point_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value / scalar; });
            return result;
        }

        /*!
         * Scale this point by 1/\p scalar
         * @tparam T Type of \p scalar. Must be an arithmetic type
         * @param scalar value to scale this vector by
         * @return a reference to this vector
         */
        template<typename T>
        Point_X<N>& operator/=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            if(scalar == 0.0)
            {
                throw std::domain_error("Cannot divide a point by 0");
            }
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value / scalar; });
            return *this;
        }

        /*!
         * Point-Vector addition
         * @param rhs Vector used to translate this point
         * @return The translated point
         */
        [[nodiscard]] Point_X<N> operator+(const Vector_X<N>& rhs) const
        {
            Point_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left + right; });
            return result;
        }

        /*!
         * In-place Point-Vector addition
         * @param rhs Vector used to translate this point
         * @return a reference to this point
         */
        Point_X<N>& operator+=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left + right; });
            return *this;
        }

        /*!
         * Unary negation operator
         * @return a negated version of this point
         */
        [[nodiscard]] inline Point_X<N> operator-() const
        {
            return (*this) * -1.0;
        }

        /*!
         * Point-Point subtraction
         * @param rhs The point to subtract from this point
         * @return The result of the subtraction
         */
        [[nodiscard]] Vector_X<N> operator-(const Point_X<N>& rhs) const
        {
            Vector_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left - right; });
            return result;
        }

        /*!
         * Point-Vector subtraction. Equivalent to Point + -Vector
         * @param rhs Vector used to translate the point by
         * @return The result of the subtraction
         */
        [[nodiscard]] Point_X<N> operator-(const Vector_X<N>& rhs) const
        {
            Point_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left - right; });
            return result;
        }

        /*!
         * In-place Point-Vector subtraction. Equivalent to Point += -Vector
         * @param rhs Vector used to translate this point by
         * @return a reference to this point
         */
        Point_X<N>& operator-=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left - right; });
            return *this;
        }

        /*!
         * @param rhs point to check equality against
         * @return true if \p rhs and this point are equal. false otherwise
         */
        [[nodiscard]] bool operator==(const Point_X<N>& rhs) const
        {
            bool isEqual = true;
            for(int i = 0; isEqual && i < N; i++) {
                isEqual = (m_values[i] == rhs[i]) && isEqual;
            }
            return isEqual;
        }

        /*!
         * @param rhs point to check inequality against
         * @return true if \p rhs and this point are not equal. false otherwise
         */
        [[nodiscard]] inline bool operator!=(const Point_X<N>& rhs) const
        {
            return !(*this == rhs);
        }

        /*!
         * @return the string representation of this point.
         */
        [[nodiscard]] std::string to_string() const
        {
            std::string output = "{ ";
            for(int i = 0; i < (N - 1); i++) {
                output += std::to_string(m_values[i]) + ", ";
            }
            if(N > 0) {
                output += std::to_string(m_values[N - 1]);
            }
            output += " }";
            return output;
        }

        /*!
         * ostream insertion operator
         * @param out ostream to insert \p rhs into
         * @param rhs point to be insert into \p out
         * @return a reference to \p out
         */
        friend std::ostream& operator<<(std::ostream& out, const Point_X<N>& rhs)
        {
            out << rhs.to_string();
            return out;
        }

        /*!
         * Square each element of this point
         * @return a reference to this point
         */
        Point_X<N>& square()
        {
            std::transform(cbegin(), cend(), begin(), [](auto value) { return value * value; });
            return *this;
        }

        /*!
         * @return a squared copy of this point
         */
        [[nodiscard]] inline Point_X<N> getSquared() const
        {
            Point_X<N> result {*this};
            return result.square();
        }

        /*!
         * @return the sum of each element of this point
         */
        [[nodiscard]] inline double sum_elements() const
        {
            return std::accumulate(cbegin(), cend(), 0);
        }

        /*!
         * Returns a point of dimension \p M. If \p M is smaller than this point's size, any extra values from this point
         * will be truncated. If \p M is large than this point's size, all extra values will be filled with 0.0.
         * @tparam M The size of the Point to return
         * @return The new point
         */
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

        /*!
         * @return A vector filled with the values in this point
         */
        [[nodiscard]] inline Vector_X<N> to_Vector() const
        {
            return Vector_X<N>{m_values};
        }

        /*!
         * Interpolates between \p a and \p b, by the factor \p t. A \p t value of 0.0 will return the point \p a, and a
         * \p t value of 1.0 will return the point \p b
         * @param a The first point to interpolate between
         * @param b The second point to interpolate between
         * @param t the interpolation value. must be between 0.0 and 1.0
         * @return The interpolated point
         */
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