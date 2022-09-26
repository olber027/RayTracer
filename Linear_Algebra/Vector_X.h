//
// Created by olber on 8/10/2022.
//

#pragma once
#include <cstdlib>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <iosfwd>
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
        std::array<double, N> m_values{};

    public:
        Vector_X() = default;

        template <typename... InitialValues>
        explicit constexpr Vector_X(InitialValues... initialValues)
        {
            static_assert(N == sizeof...(InitialValues), "Incorrect number of parameters given to constructor");
            static_assert((std::is_convertible_v<InitialValues, double> && ...), "Parameters must be implicitly convertible to doubles");
            m_values = std::array<double, N>{initialValues...};
        }

        constexpr Vector_X(std::initializer_list<double> initialValues)
        {
            if(initialValues.size() != N) {
                throw std::invalid_argument("Vector_X constructor expected a initializer_list of size " + std::to_string(N) + ", but got size " + std::to_string(initialValues.size()));
            }
            std::copy_n(std::begin(initialValues), N, begin());
        }

        explicit constexpr Vector_X(const std::array<double, N>& initialValues) : m_values{initialValues} { }
        explicit constexpr Vector_X(const std::array<double, N>&& initialValues) : m_values{initialValues} { }
        explicit constexpr Vector_X(const double (&initialValues)[N]) {
            std::copy_n(std::begin(initialValues), N, begin());
        }
        explicit Vector_X(const std::vector<double>& initialValues) {
            if(initialValues.size() != N) {
                throw std::invalid_argument("Vector_X constructor expected a std::vector of size " + std::to_string(N) + ", but got size " + std::to_string(initialValues.size()));
            }
            std::copy_n(std::begin(initialValues), N, begin());
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
        [[nodiscard]] auto cbegin()  const { return std::cbegin(m_values);  }
        [[nodiscard]] auto cend()    const { return std::cend(m_values);    }
        [[nodiscard]] auto crbegin() const { return std::crbegin(m_values); }
        [[nodiscard]] auto crend()   const { return std::crend(m_values);   }

        /*!
         * The dot product of two vectors. Vectors must be the same size
         * @param rhs The other vector used to calculate the dot product
         * @return the dot product of this vector and \p rhs
         */
        [[nodiscard]] inline double dot(const Vector_X<N>& rhs) const
        {
            return (*this) * rhs;
        }

        /*!
         * The cross product of two vectors. Both vectors must be of size 3.
         * @param rhs the other vector used to calculate the cross product
         * @return The cross product of this vector and \p rhs
         */
        [[nodiscard]] Vector_X<N> cross(const Vector_X<N>& rhs) const
        {
            static_assert(N == 3, "cross product can only be computed on 3 dimensional vectors");
            return {
                m_values[1] * rhs[2] - m_values[2] * rhs[1],
                m_values[2] * rhs[0] - m_values[0] * rhs[2],
                m_values[0] * rhs[1] - m_values[1] * rhs[0]
            };
        }

        /*!
         * Fill the vector with the given \p value
         * @param value values used to fill the vector
         * @return a reference to this vector
         */
        Vector_X<N>& fill(double value)
        {
            std::fill(begin(), end(), value);
            return (*this);
        }

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
         * Calculate the dot product of two vectors. Vectors must be the same size.
         * @param rhs the other vector used to calculate the dot product
         * @return The dot product of this vector and \p rhs
         */
        [[nodiscard]] double operator*(const Vector_X<N>& rhs) const
        {
            double result = 0;
            for(int i = 0; i < N; i++) {
                result += m_values[i] * rhs[i];
            }
            return result;
        }

        /*!
         * Return a vector that's the result of scaling this vector by \p scalar
         * @tparam T The type of \p scalar. Must be an Arithmetic type.
         * @param scalar value to scale the Vector by
         * @return The scaled vector
         */
        template<typename T>
        [[nodiscard]] Vector_X<N> operator*(T scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            Vector_X<N> result;
            std::transform(cbegin(), cend(), result.begin(), [scalar](auto value) { return value * scalar; });
            return result;
        }

        /*!
         * Scale \p rhs by \p scalar
         * @tparam T The type of \p scalar. Must be an arithmetic type
         * @param scalar value to scale \p rhs by.
         * @param rhs vector to be scaled.
         * @return the scaled vector
         */
        template<typename T>
        [[nodiscard]] friend inline Vector_X<N> operator*(T scalar, const Vector_X<N>& rhs)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return rhs * scalar;
        }

        /*!
         * Scale this vector by the given \p scalar
         * @tparam T The type of the \p scalar. Must be an arithmetic type.
         * @param scalar The value to scale this vector by.
         * @return a reference to this vector
         */
        template<typename T>
        Vector_X<N>& operator*=(T scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            std::transform(cbegin(), cend(), begin(), [scalar](auto value) { return value * scalar; });
            return *this;
        }

        /*!
         * Returns a vector containing result of scaling this vector by 1/\p scalar
         * @tparam T The type of \p scalar. Must be an arithmetic type
         * @param scalar The value to scale this vector by. Cannot be 0
         * @return the scaled vector
         */
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

        /*!
         * Scale this vector by 1/\p scalar.
         * @tparam T The type of the \p scalar. Must be an arithmetic type.
         * @param scalar The value to scale this vector by. Cannot be 0
         * @return a reference to this vector
         */
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

        /*!
         * Returns a vector that is the result of adding this vector to \p rhs. Vectors must be the same size.
         * @param rhs Vector to add this vector to.
         * @return The result of the vector addition.
         */
        [[nodiscard]] Vector_X<N> operator+(const Vector_X<N>& rhs) const
        {
            Vector_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left + right; });
            return result;
        }

        /*!
         * Adds \rhs to this vector.
         * @param rhs Vector to add to this vector
         * @return a reference to this vector
         */
        Vector_X<N>& operator+=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left + right; });
            return *this;
        }

        /*!
         * Unary negation operator. Multiples this vector by -1.0.
         * @return a reference to this vector
         */
        [[nodiscard]] inline Vector_X<N> operator-() const
        {
            return (*this) * -1.0;
        }

        /*!
         * Returns the result of subtracting \p rhs from this vector.
         * @param rhs The vector to subtract
         * @return The result of the vector subtraction.
         */
        [[nodiscard]] Vector_X<N> operator-(const Vector_X<N>& rhs) const
        {
            Vector_X<N> result;
            std::transform(cbegin(), cend(), rhs.cbegin(), result.begin(), [](auto left, auto right) { return left - right; });
            return result;
        }

        /*!
         * Subtract \p rhs from this vector
         * @param rhs The vector to subtract
         * @return a reference to this vector
         */
        Vector_X<N>& operator-=(const Vector_X<N>& rhs)
        {
            std::transform(cbegin(), cend(), rhs.cbegin(), begin(), [](auto left, auto right) { return left - right; });
            return *this;
        }

        /*!
         * Checks if each element of this vector is equal to the corresponding element of \p rhs
         * @param rhs The vector to compare against
         * @return true if the two vectors are equal, false otherwise.
         */
        [[nodiscard]] bool operator==(const Vector_X<N>& rhs) const
        {
            bool isEqual = true;
            for(int i = 0; isEqual && i < N; i++) {
                isEqual = (m_values[i] == rhs[i]) && isEqual;
            }
            return isEqual;
        }

        /*!
         * Checks if any element of this vector differ from corresponding element of \p rhs
         * @param rhs The vector to compare this to
         * @return true if the two vectors are different, false otherwise.
         */
        [[nodiscard]] inline bool operator!=(const Vector_X<N>& rhs) const
        {
            return !(*this == rhs);
        }

        /*!
         * @return the summation of the square of each value of this vector
         */
        [[nodiscard]] inline double getMagnitudeSquared() const
        {
            double result = 0;
            for(int i = 0; i < N; i++) {
                result += m_values[i] * m_values[i];
            }
            return result;
        }

        /*!
         * @return The magnitude of this vector
         */
        [[nodiscard]] inline double getMagnitude() const
        {
            return std::sqrt(getMagnitudeSquared());
        }

        /*!
         * Normalizes this vector by dividing each element by the magnitude of the vector
         * @return a reference to this vector
         */
        inline Vector_X<N>& normalize()
        {
            (*this) /= getMagnitude();
            return *this;
        }

        /*!
         * @return a normalized copy of this vector
         */
        [[nodiscard]] inline Vector_X<N> getUnitVector() const
        {
            return (*this) / getMagnitude();
        }

        /*!
         * Square each element of this vector
         * @return a reference to this vector
         */
        Vector_X<N>& square()
        {
            std::transform(cbegin(), cend(), begin(), [](auto value) { return value * value; });
            return (*this);
        }

        /*!
         * @return a squared copy of this vector
         */
        [[nodiscard]] inline Vector_X<N> getSquared() const
        {
            Vector_X<N> result {*this};
            return result.square();
        }

        /*!
         * @return the summation of each element of this vector
         */
        [[nodiscard]] inline double sum_elements() const
        {
            return std::accumulate(cbegin(), cend(), 0);
        }

        /*!
         * Returns a vector of dimension \p M. If M is smaller than this vector's size, any extra values from this vector
         * will be truncated. If M is large than this vectors size, all extra values will be filled with 0.0.
         * @tparam M The size of the Vector to return
         * @return The new vector
         */
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

        /*!
         * Projects this vector onto \p other.
         * @param other vector for this to be projected onto.
         * @return The projected vector
         */
        [[nodiscard]] Vector_X<N> projectOnto(const Vector_X<N>& other) const
        {
            Vector_X<N> unit_other = other.getUnitVector();
            return unit_other * ((*this) * unit_other);
        }

        /*!
         * Returns the angle between this vector and \p other in radians.
         * @param other Vector to get the angle between
         * @return the angle between this vector and \p other.
         */
        [[nodiscard]] inline double getAngleBetween(const Vector_X<N>& other) const
        {
            return acos(((*this) * other) / (getMagnitude() * other.getMagnitude()));
        }

        /*!
         * @return the string representation of this vector.
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
         * Adds a string representation of \p rhs to \p out
         * @param out the ostream to add \p rhs to
         * @param rhs the vector to add to \p out
         * @return a reference to \p out
         */
        friend std::ostream& operator<<(std::ostream& out, const Vector_X<N>& rhs)
        {
            out << rhs.to_string();
            return out;
        }

        /*!
         * Returns the triple scalar product of \p a, \p b, and \p c, as calculated by a.dot(b.cross(c));
         * @param a The \p a vector in the above formula
         * @param b the \p b vector in the above formula
         * @param c the \p c vector in the above formula
         * @return the triple scalar product of \p a, \p b, and \p c
         */
        [[nodiscard]] static inline double triple_scalar_product(const Vector_X<N>& a, const Vector_X<N>& b, const Vector_X<N>& c)
        {
            static_assert(N == 3, "triple scalar product can only be computed on 3 dimensional vectors");
            return a * b.cross(c);
        }

        /*!
         * Interpolates between \p a and \p b, by the factor \p t. A \p t value of 0.0 will return the vector \p a, and a
         * \p t value of 1.0 will return the vector \p b
         * @param a The first vector to interpolate between
         * @param b The second vector to interpolate between
         * @param t the interpolation value. must be between 0.0 and 1.0
         * @return The interpolated vector
         */
        [[nodiscard]] static Vector_X<N> linear_interpolation(const Vector_X<N>& a, const Vector_X<N>& b, double t)
        {
            if(t > 1.0 || t < 0.0)
            {
                throw std::invalid_argument("t must be in the range of [0.0, 1.0]. Given value was: " + std::to_string(t));
            }
            return (a * (1.0 - t)) + (b * t);
        }
    };

    using Vector_3 = Vector_X<3>;
    using Vector_4 = Vector_X<4>;

    [[nodiscard]] static Vector_3 getUnitX() { return {1.0, 0.0, 0.0}; }
    [[nodiscard]] static Vector_3 getUnitY() { return {0.0, 1.0, 0.0}; }
    [[nodiscard]] static Vector_3 getUnitZ() { return {0.0, 0.0, 1.0}; }
}