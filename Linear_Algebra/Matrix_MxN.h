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

        template<size_t Column, size_t... Rows>
        constexpr Vector_X<N> getColumnHelper(std::index_sequence<Rows...>) const {
            Vector_X<N> result;
            ((result.template getValue<Rows>() = getValue<Rows, Column>()), ...);
            return result;
        }

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
        [[nodiscard]] auto cbegin()  const { return std::cbegin(m_values);  }
        [[nodiscard]] auto cend()    const { return std::cend(m_values);    }
        [[nodiscard]] auto crbegin() const { return std::crbegin(m_values); }
        [[nodiscard]] auto crend()   const { return std::crend(m_values);   }

        /*!
         * @param index index of the desired row
         * @return A copy of the row at the given \p index
         */
        [[nodiscard]] inline Vector_X<N> operator[](size_t index) const { return m_values[index]; }
        /*!
         * @param index index of the desired row
         * @return A reference to the row at the given \p index
         */
        [[nodiscard]] inline Vector_X<N>& operator[](size_t index) { return m_values[index]; }

        /*!
         * Exception-throwing version of the index operation
         * @param index index of the desired row
         * @return A copy of the row at the given \p index
         */
        [[nodiscard]] inline Vector_X<N> at(size_t index) const { return m_values.at(index); }
        /*!
         * Exception-throwing version of the index operation
         * @param index index of the desired row
         * @return A reference to the row at the given \p index
         */
        [[nodiscard]] inline Vector_X<N>& at(size_t index) { return m_values.at(index); }

        /*!
         * Exception-throwing double indexing operation
         * @param row index of the desired row
         * @param column index of the desired column
         * @return a copy of the value at the given \p row and \p column
         */
        [[nodiscard]] inline double at(size_t row, size_t column) const { return m_values.at(row).at(column); }
        /*!
         * Exception-throwing double indexing operation
         * @param row index of the desired row
         * @param column index of the desired column
         * @return a reference to the value at the given \p row and \p column
         */
        [[nodiscard]] inline double& at(size_t row, size_t column) { return m_values.at(row).at(column); }

        /*!
         * Compile-time double index operation
         * @tparam Row index of the desired row
         * @tparam Column index of the desired column
         * @return a copy of the value at the given \p row and \p column
         */
        template<size_t Row, size_t Column>
        [[nodiscard]] constexpr inline double getValue() const
        {
            static_assert(Row < M, "Row index was out of bounds");
            static_assert(Column < N, "Column index was out of bounds");
            return std::get<Row>(m_values).template getValue<Column>();
        }

        /*!
         * Compile-time double index operation
         * @tparam Row index of the desired row
         * @tparam Column index of the desired column
         * @return A reference to the value at the given \p row and \p column
         */
        template<size_t Row, size_t Column>
        [[nodiscard]] constexpr inline double& getValue()
        {
            static_assert(Row < M, "Row index was out of bounds");
            static_assert(Column < N, "Column index was out of bounds");
            return std::get<Row>(m_values).template getValue<Column>();
        }

        /*!
         * Compile-time Row access
         * @tparam Index index of the desired row
         * @return a copy of the row at the given \p Index
         */
        template<size_t Index>
        [[nodiscard]] constexpr inline Vector_X<M> getRow() const
        {
            static_assert(Index < M, "Row index was out of bounds");
            return std::get<Index>(m_values);
        }

        /*!
         * Compile-time Row access
         * @tparam Index index of the desired row
         * @return a reference to the row at the given \p Index
         */
        template<size_t Index>
        [[nodiscard]] constexpr inline Vector_X<M>& getRow()
        {
            static_assert(Index < M, "Row index was out of bounds");
            return std::get<Index>(m_values);
        }

        /*!
         * Explicit row access. Mostly just here for code clarity purposes. Throws an exception if \p index is out of bounds
         * @param index index of the desired row
         * @return A copy of the row at the given \p index
         */
        [[nodiscard]] inline Vector_X<M> getRow(size_t index) const { return m_values.at(index); }
        /*!
         * Explicit row access. Mostly just here for code clarity purposes. Throws an exception if \p index is out of bounds
         * @param index index of the desired row
         * @return A reference to the row at the given \p index
         */
        [[nodiscard]] inline Vector_X<M>& getRow(size_t index) { return m_values.at(index); }

        /*!
         * Compile-time column accessor
         * @tparam Index index of the desired column
         * @return A copy of the column at the given \p Index
         */
        template<size_t Index>
        [[nodiscard]] constexpr Vector_X<N> getColumn() const
        {
            static_assert(Index < N, "Column index was out of bounds");
            return getColumnHelper<Index>(std::make_index_sequence<N>{});
        }

        /*!
         * Column accessor. Will throw an exception of \p index is out of bounds
         * @param index index of the desired column
         * @return A copy of the column at the given \p index
         */
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

        /*!
         * @param rhs Matrix to multiply by
         * @return The result of the multiplication
         */
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

        /*!
         * @param rhs Matrix to multiply this matrix by
         * @return a reference to this matrix
         */
        inline Matrix_MxN<M, N>& operator*=(const Matrix_MxN<N, M>& rhs)
        {
            static_assert(M == N, "Can only perform operator*= on square matrices, otherwise the dimensions don't match.");
            (*this) = (*this) * rhs;
            return (*this);
        }

        /*!
         * Row-vector multiplication
         * @param lhs Vector to be multiplied
         * @param rhs Matrix to multiply the vector by
         * @return The resulting vector
         */
        [[nodiscard]] friend Vector_X<N> operator*(const Vector_X<M>& lhs, const Matrix_MxN<M, N>& rhs)
        {
            Vector_X<N> result;
            for(int i = 0; i < N; i++) {
                result[i] = lhs * rhs.getColumn(i);
            }
            return result;
        }

        /*!
         * Column-Vector multiplication
         * @param rhs The vector to multiply by
         * @return The result of the multiplication
         */
        [[nodiscard]] Vector_X<M> operator*(const Vector_X<N>& rhs) const
        {
            Vector_X<M> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] * rhs;
            }
            return result;
        }

        /*!
         * @tparam T Type of \p scalar. Must be an arithmetic type
         * @param scalar value to scale the matrix by
         * @return The scaled matrix
         */
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

        /*!
         * @tparam T Type of \p scalar. Must be an arithmetic type.
         * @param scalar Value to scale the matrix by.
         * @return A reference to this matrix
         */
        template<typename T>
        [[nodiscard]] Matrix_MxN<M, N>& operator*=(const T& scalar)
        {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            for(auto& row : m_values) {
                row *= scalar;
            }
            return *this;
        }

        /*!
         * @param rhs Matrix to add to this
         * @return The result of the addition
         */
        [[nodiscard]] Matrix_MxN<M, N> operator+(const Matrix_MxN<M, N>& rhs) const
        {
            Matrix_MxN<M, N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] + rhs[i];
            }
            return result;
        }

        /*!
         * @param rhs matrix to add to this one
         * @return a reference to this matrix
         */
        Matrix_MxN<M, N>& operator+=(const Matrix_MxN<M, N>& rhs) {
            for(int i = 0; i < M; i++) {
                m_values[i] += rhs[i];
            }
            return (*this);
        }

        /*!
         * @param rhs matrix to subtract
         * @return the result of the subtraction
         */
        [[nodiscard]] Matrix_MxN<M, N> operator-(const Matrix_MxN<M, N>& rhs) const
        {
            Matrix_MxN<M, N> result;
            for(int i = 0; i < M; i++) {
                result[i] = m_values[i] - rhs[i];
            }
            return result;
        }

        /*!
         * @param rhs Matrix to subtract this one by
         * @return a reference to this matrix
         */
        Matrix_MxN<M, N>& operator-=(const Matrix_MxN<M, N>& rhs)
        {
            for(int i = 0; i < M; i++) {
                m_values[i] -= rhs[i];
            }
            return (*this);
        }

        /*!
         * @return the string representation of this matrix
         */
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

        /*!
         * ostream operation for matrices
         * @param out ostream to place the matrix into
         * @param rhs matrix to be serialized
         * @return a reference to \p out
         */
        friend std::ostream& operator<<(std::ostream& out, const Matrix_MxN<M, N>& rhs)
        {
            out << rhs.to_string();
            return out;
        }

        /*!
         * @return A transposed version of this matrix
         */
        Matrix_MxN<N, M> getTransposed() const
        {
            Matrix_MxN<N, M> result;
            for(int i = 0; i < N; i++) {
                result[i] = getColumn(i);
            }
            return result;
        }

        /*!
         * In-place matrix transposition. Matrix must be square.
         * @return a reference to this matrix
         */
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