#pragma once
#include "Vector_X.h"
#include "Point_X.h"

namespace linear_algebra_core
{
    class Ray
    {
    private:
        using value_type = double;

        Point_X<3, value_type>  m_origin;
        Vector_X<3, value_type> m_direction;

    public:
        Ray() = default;
        explicit Ray(const Point_X<3, value_type>& start, const Vector_X<3, value_type>& dir) : m_origin(start), m_direction(dir) { }
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
        template<DoesNotNarrowlyConvertTo<value_type> T>
        Point_X<3, value_type> operator*(T scalar) const {
            return m_origin + (m_direction * scalar);
        }

        /*!
         * Calculate the point at getOrigin() + (\p scalar * getDirection())
         * @tparam T Type of \p scalar. Must be an arithmetic type.
         * @param scalar value to scale the direction by.
         * @return The point returned by the above calculation
         */
        template<DoesNotNarrowlyConvertTo<value_type> T>
        Point_X<3, value_type> at(T scalar) const {
            return (*this) * scalar;
        }

        /*!
         * @return the origin of the ray
         */
        [[maybe_unused]] const Point_X<3, value_type>& getOrigin() { return m_origin; }
        /*!
         * @return the origin of the ray
         */
        [[nodiscard]] Point_X<3, value_type> getOrigin() const { return m_origin; }
        /*!
         * Sets the origin to \p new_origin
         * @param new_origin the new origin for the ray
         */
        [[maybe_unused]] void setOrigin(const Point_X<3, value_type>& new_origin) { m_origin = new_origin; }

        /*!
         * @return the direction of the ray
         */
        const Vector_X<3, value_type>& getDirection() { return m_direction; }
        /*!
         * @return the direction of the ray
         */
        [[nodiscard]] Vector_X<3, value_type> getDirection() const { return m_direction; }
        /*!
         * sets the direction of the ray to the \p new_direction
         * @param new_direction the new direction for the ray
         */
        [[maybe_unused]] void setDirection(const Vector_X<3, value_type>& new_direction) { m_direction = new_direction; }
    };
}