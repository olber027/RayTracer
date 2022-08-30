#pragma once
#include "Vector_X.h"
#include "Point_X.h"

namespace linear_algebra_core
{
    class Ray
    {
    private:
        Point_3  origin;
        Vector_3 direction;

    public:
        Ray() = default;
        explicit Ray(const Point_3& start, const Vector_3& dir) : origin(start), direction(dir) { }
        ~Ray() = default;
        Ray(const Ray& other) = default;
        Ray(Ray&& other) noexcept = default;
        Ray& operator=(const Ray& other) = default;
        Ray& operator=(Ray&& other) noexcept = default;

        template<typename T>
        Point_3 operator*(T scalar) const {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return origin + (direction * scalar);
        }

        template<typename T>
        Point_3 at(T scalar) const {
            return (*this) * scalar;
        }

        [[nodiscard]] Point_3 getOrigin() const
        {
            return origin;
        }

        [[nodiscard]] Vector_3 getDirection() const
        {
            return direction;
        }
    };
}