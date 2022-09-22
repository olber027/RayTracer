#pragma once
#include "Vector_X.h"
#include "Point_X.h"

namespace linear_algebra_core
{
    class Ray
    {
    private:
        Point_3  m_origin;
        Vector_3 m_direction;

    public:
        Ray() = default;
        explicit Ray(const Point_3& start, const Vector_3& dir) : m_origin(start), m_direction(dir) { }
        ~Ray() = default;
        Ray(const Ray& other) = default;
        Ray(Ray&& other) noexcept = default;
        Ray& operator=(const Ray& other) = default;
        Ray& operator=(Ray&& other) noexcept = default;

        /*!
         * Calculate the point at getOrigin() + (\p scalar * getDirection())
         * @tparam T Type of \p scalar. Must be an arithmetic type.
         * @param scalar value to scale the direction by.
         * @return The point returned by the above calculation
         */
        template<typename T>
        Point_3 operator*(T scalar) const {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return m_origin + (m_direction * scalar);
        }

        /*!
         * Calculate the point at getOrigin() + (\p scalar * getDirection())
         * @tparam T Type of \p scalar. Must be an arithmetic type.
         * @param scalar value to scale the direction by.
         * @return The point returned by the above calculation
         */
        template<typename T>
        Point_3 at(T scalar) const {
            return (*this) * scalar;
        }

        /*!
         * @return the origin of the ray
         */
        const Point_3& getOrigin() { return m_origin; }
        /*!
         * @return the origin of the ray
         */
        [[nodiscard]] Point_3 getOrigin() const { return m_origin; }
        /*!
         * Sets the origin to \p new_origin
         * @param new_origin the new origin for the ray
         */
        void setOrigin(const Point_3& new_origin) { m_origin = new_origin; }

        /*!
         * @return the direction of the ray
         */
        const Vector_3& getDirection() { return m_direction; }
        /*!
         * @return the direction of the ray
         */
        [[nodiscard]] Vector_3 getDirection() const { return m_direction; }
        /*!
         * sets the direction of the ray to the \p new_direction
         * @param new_direction the new direction for the ray
         */
        void setDirection(const Vector_3& new_direction) { m_direction = new_direction; }
    };
}