#pragma once
#include <optional>
#include "Point_X.h"
#include "Ray.h"

namespace geometry
{
    using namespace linear_algebra_core;

    class Geometry
    {
    public:
        [[nodiscard]] virtual bool intersects(const Ray& ray) const = 0;
        [[nodiscard]] virtual std::optional<Point_3> intersectsAt(const Ray& ray) const = 0;
    };
}