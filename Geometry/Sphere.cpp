//
// Created by olber on 8/27/2022.
//

#include "Sphere.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "json.h"

namespace geometry {
    using namespace linear_algebra_core;

    bool Sphere::intersects(const Ray& ray) const
    {
        Vector_3 ray_origin_to_sphere_center = m_center - ray.getOrigin();
        Vector_3 sphere_center_to_ray_origin = ray.getOrigin() - m_center;
        double normalized_radius = m_radius / std::sqrt(3);
        Vector_3 perpendicular_vector = ray_origin_to_sphere_center.cross(ray_origin_to_sphere_center + Vector_3().fill(normalized_radius));
        Point_3 sphere_edge = m_center + perpendicular_vector;
        double max_value_while_still_intersecting = (ray.getOrigin() - sphere_edge) * (ray_origin_to_sphere_center);
        double actual_value = ray.getDirection() * ray_origin_to_sphere_center;

        double a = ray.getDirection() * ray.getDirection();
        double b = 2 * ray.getDirection() * sphere_center_to_ray_origin;
        double c = sphere_center_to_ray_origin * sphere_center_to_ray_origin - m_radius * m_radius;
        double discriminant = (b * b - 4 * a * c);
        return max_value_while_still_intersecting > actual_value;
    }

    std::optional<Point_3> Sphere::intersectsAt(const Ray& ray) const
    {
        return {};
    }

    void Sphere::fromJson(const nlohmann::json& json_node)
    {
        // TODO: Verify this works.
        std::vector<double> center = json_node.at("center");
        if(center.size() != 3) {
            throw std::invalid_argument("center must be specified in the form: [X, Y, Z]");
        }
        m_center = Point_3(center);

        double radius = json_node.at("radius");
        if(radius < 0) {
            throw std::invalid_argument("radius must be greater than 0");
        }
        m_radius = radius;
    }
}
