//
// Created by olber on 7/14/2022.
//

#pragma once

#include <array>
#include <vector>
#include "Point_X.h"
#include "Vector_X.h"

namespace linear_algebra_core {

    template<size_t M, size_t N>
    class Matrix_MxN {
    private:
        std::array<Vector_X<N>, M> m_values;

        template<size_t... Rows>
        Matrix_MxN(const double(&initialValues)[M][N], std::index_sequence<Rows...> rows) : m_values{Vector_X<N>(initialValues[Rows])... } { }

    public:
        Matrix_MxN() : m_values{} { }

        explicit Matrix_MxN(const double (&initialValues)[M][N]) : Matrix_MxN(initialValues, std::make_index_sequence<M>()) { }

        ~Matrix_MxN() = default;
        Matrix_MxN(const Matrix_MxN<M, N>& other) = default;
        Matrix_MxN(Matrix_MxN<M, N>&& other) noexcept = default;
        Matrix_MxN<M, N>& operator=(const Matrix_MxN<M, N>& other) = default;
        Matrix_MxN<M, N>& operator=(Matrix_MxN<M, N>&& other) noexcept = default;

        auto begin()   { return std::begin(m_values);   }
        auto end()     { return std::end(m_values);     }
        auto rbegin()  { return std::rbegin(m_values);  }
        auto rend()    { return std::rend(m_values);    }
        auto cbegin()  const { return std::cbegin(m_values);  }
        auto cend()    const { return std::cend(m_values);    }
        auto crbegin() const { return std::crbegin(m_values); }
        auto crend()   const { return std::crend(m_values);   }

        [[nodiscard]] inline Vector_X<N> operator[](size_t index) const { return m_values[index]; }
        [[nodiscard]] inline Vector_X<N>& operator[](size_t index) { return m_values[index]; }

        [[nodiscard]] inline Vector_X<N> at(size_t index) const { return m_values.at(index); }
        [[nodiscard]] inline Vector_X<N>& at(size_t index) { return m_values.at(index); }

        [[nodiscard]] inline double at(size_t row, size_t column) const { return m_values.at(row).at(column); }
        [[nodiscard]] inline double& at(size_t row, size_t column) { return m_values.at(row).at(column); }

        template<size_t index>
        [[nodiscard]] inline Vector_X<M> getRow() const
        {
            static_assert(index < M, "index was out of bounds");
            return m_values[index];
        }

        template<size_t index>
        [[nodiscard]] inline Vector_X<M>& getRow()
        {
            static_assert(index < M, "index was out of bounds");
            return m_values[index];
        }

        [[nodiscard]] inline Vector_X<M> getRow(size_t index) const
        {
            if(index >= M) {
                throw std::out_of_range("given index (" + std::to_string(index) + ") was out of range.");
            }
            return m_values[index];
        }

        [[nodiscard]] inline Vector_X<M>& getRow(size_t index)
        {
            if(index >= M) {
                throw std::out_of_range("given index (" + std::to_string(index) + ") was out of range.");
            }
            return m_values[index];
        }

        template<size_t index>
        [[nodiscard]] constexpr Vector_X<N> getColumn() const
        {
            static_assert(index < N);
            Vector_X<N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i][index];
            }
            return result;
        }

        [[nodiscard]] Vector_X<N> getColumn(size_t index) const
        {
            if(index >= N) {
                throw std::out_of_range("given index (" + std::to_string(index) + ") was out of range.");
            }
            Vector_X<N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i][index];
            }
            return result;
        }

        [[nodiscard]] Matrix_MxN<M, M> operator*(const Matrix_MxN<N, M>& rhs) const
        {
            Matrix_MxN<M, M> result;
            for(int i = 0; i < M; i++) {
                for(int j = 0; j < N; j++) {
                    result[i][j] = m_values[i] * rhs.getColumn(j);
                }
            }
            return result;
        }

        inline Matrix_MxN<M, N> operator*=(const Matrix_MxN<N, M>& rhs)
        {
            static_assert(M == N, "Can only perform operator*= on square matrices, otherwise the dimensions don't match.");
            (*this) = (*this) * rhs;
            return (*this);
        }

        // row vector
        [[nodiscard]] friend Vector_X<N> operator*(const Vector_X<M>& lhs, const Matrix_MxN<M, N>& rhs)
        {
            Vector_X<N> result;
            for(int i = 0; i < N; i++) {
                result[i] = lhs * rhs.getColumn(i);
            }
            return result;
        }

        // column vector
        [[nodiscard]] Vector_X<M> operator*(const Vector_X<N>& rhs) const
        {
            Vector_X<M> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] * rhs;
            }
            return result;
        }

        template<typename T>
        [[nodiscard]] Matrix_MxN<M, N> operator*(const T& scalar) const
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            Matrix_MxN<M, N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] * scalar;
            }
            return result;
        }

        template<typename T>
        [[nodiscard]] Matrix_MxN<M, N>& operator*=(const T& scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            for(auto& row : m_values) {
                row *= scalar;
            }
            return *this;
        }

        [[nodiscard]] Matrix_MxN<M, N> operator+(const Matrix_MxN<M, N>& rhs) const
        {
            Matrix_MxN<M, N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] + rhs[i];
            }
            return result;
        }

        Matrix_MxN<M, N>& operator+=(const Matrix_MxN<M, N>& rhs) {
            for(int i = 0; i < M; i++) {
                m_values[i] += rhs[i];
            }
            return (*this);
        }

        [[nodiscard]] Matrix_MxN<M, N> operator-(const Matrix_MxN<M, N>& rhs) const
        {
            Matrix_MxN<M, N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] - rhs[i];
            }
            return result;
        }

        Matrix_MxN<M, N>& operator-=(const Matrix_MxN<M, N>& rhs)
        {
            for(int i = 0; i < M; i++) {
                m_values[i] -= rhs[i];
            }
            return (*this);
        }

        [[nodiscard]] std::string to_string() const
        {
            std::string result = "{ \n";
            for(int i = 0; i < (M - 1); i++) {
                result += "\t" + m_values[i] + ",\n";
            }
            if(M > 0) {
                result + "\t" + m_values[M - 1] + "\n";
            }
            result + "}";
            return result;
        }

        friend std::ostream& operator<<(std::ostream& out, const Matrix_MxN<M, N>& rhs)
        {
            out << rhs.to_string();
            return out;
        }

        Matrix_MxN<N, M> getTransposed() const
        {
            Matrix_MxN<N, M> result;
            for(int i = 0; i < N; i++) {
                result[i] = getColumn(i);
            }
            return result;
        }

        Matrix_MxN<M, N>& transpose()
        {
            static_assert(M == N, "cannot perform an inplace transpose on a non-square matrix");
            for(int i = 1; i < M; i++) {
                for(int j = 0; j < i; j++) {
                    std::swap(m_values[i][j], m_values[j][i]);
                }
            }
            return (*this);
        }
    };
    using Matrix_3x3 = Matrix_MxN<3, 3>;
    using Matrix_4x4 = Matrix_MxN<4, 4>;
}