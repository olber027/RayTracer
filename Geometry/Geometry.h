#pragma once
#include <optional>
#include "LinearAlgebraTypeTraits.h"
#include "Point_X.h"
#include "Ray.h"
#include "Color.h"
#include "json.h"

namespace geometry
{
    using namespace linear_algebra_core;

    template<IsFloatingPoint value_type>
    class Geometry
    {
    public:
        using Ray_3 = Ray<3, value_type>;
        using Point_3 = Point_X<3, value_type>;
        using Vector_3 = Vector_X<3, value_type>;

                      virtual ~Geometry() = default;
        [[nodiscard]] virtual bool intersects(const Ray_3& ray) const = 0;
        [[nodiscard]] virtual std::optional<Point_3> getIntersectionPoint(const Ray_3& ray) const = 0;
        // TODO: currently just assuming the given point is retrieved from the getIntersectionPoint function... figure out a better way to do this.
        [[nodiscard]] virtual color_core::Color getColorAt(const Point_3& point) const = 0;
        [[nodiscard]] virtual Vector_3 getNormalAt(const Point_3& point) const = 0;
                      virtual void fromJson(const nlohmann::json& json_node) = 0;
    };
}
