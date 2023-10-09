//
// Created by olber on 8/27/2022.
//

#include "Sphere.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"
#include "LinearAlgebraJsonParser.h"

namespace geometry {
    using namespace linear_algebra_core;
    using namespace color_core;

    bool Sphere::intersects(const Ray& ray) const
    {
        std::optional<Point_3> intersection_point = getIntersectionPoint(ray);
        return intersection_point.has_value();
    }

    std::optional<Point_3> Sphere::getIntersectionPoint(const Ray& ray) const
    {
        Ray normalized_ray = Ray(ray.getOrigin(), ray.getDirection().getUnitVector());
        Vector_3 center_to_ray_origin = m_center - ray.getOrigin();
        double t_projection_of_center_to_ray = center_to_ray_origin * normalized_ray.getDirection();
        if (t_projection_of_center_to_ray < 0) {
            return std::nullopt;
        }
        double projected_center_to_center_distance_squared = center_to_ray_origin * center_to_ray_origin - t_projection_of_center_to_ray * t_projection_of_center_to_ray;
        double radius_squared = m_radius * m_radius;
        if (projected_center_to_center_distance_squared > radius_squared)
        {
            return std::nullopt;
        }
        double t_projected_center_to_sphere_surface = std::sqrt(radius_squared - projected_center_to_center_distance_squared);
        double t_first_intersection = t_projection_of_center_to_ray - t_projected_center_to_sphere_surface;
        double t_second_intersection = t_projection_of_center_to_ray + t_projected_center_to_sphere_surface;

        if (t_first_intersection > t_second_intersection) {
            std::swap(t_first_intersection, t_second_intersection);
        }

        if (t_first_intersection < 0)
        {
            return std::nullopt;  // intersection point is behind the ray
        }
        Point_3 result = normalized_ray * t_first_intersection;
        return {result};
    }

    void Sphere::fromJson(const nlohmann::json& json_node)
    {
        double radius;
        nlohmann::json color_json, center_json;

        try {
            center_json = json_node.at("center");
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'center' key.");
        }

        try {
            radius = json_node.at("radius").get<decltype(radius)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'radius' key.");
        }
        if(radius < 0) {
            throw std::invalid_argument("radius must be greater than 0");
        }

        try {
            color_json = json_node.at("color");
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'color' key.");
        }


        m_center = utility::PointFromJson<3>(center_json);
        m_radius = radius;
        m_color.fromJson(color_json);
    }

    Color Sphere::getColorAt(const Point_3& point) const
    {
        return m_color;
    }

    Vector_3 Sphere::getNormalAt(const Point_3& point) const
    {
        return (m_center - point).normalize();
    }
}
