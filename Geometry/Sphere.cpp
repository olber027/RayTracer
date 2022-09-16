//
// Created by olber on 8/27/2022.
//

#include "Sphere.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"

namespace geometry {
    using namespace linear_algebra_core;
    using namespace color_core;

    /*!
     * Determines if the given /p ray intersects this sphere.
     * @param ray The ray to check for intersection
     * @return true if /p ray intersects this sphere. False otherwise
     */
    bool Sphere::intersects(const Ray& ray) const
    {
        std::optional<Point_3> intersection_point = getIntersectionPoint(ray);
        return intersection_point.has_value();
    }

    /*!
     * Determines if the given /p ray intersects this sphere and returns the first intersection point.
     * @param ray The ray to check for intersection
     * @return a std::optional containing the first intersection point, if it exists. containing nothing, otherwise.
     * @details uses the geometric solution described here: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
     */
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
        std::vector<double> center{0, 0, 0};
        double radius = 1.0;
        std::vector<int> color{0, 0, 0};
        auto vector_to_string = [] (auto val) { return "[" + std::to_string(val[0]) + ", " + std::to_string(val[1]) + ", " + std::to_string(val[2]) + "]"; };

        try {
            center = json_node.at("center").get<std::vector<double>>();
            if(center.size() != 3)
            {
                throw std::invalid_argument("'center' field must be specified in the form: [X, Y, Z]");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'center' key. Using " + vector_to_string(center));
        }

        try {
            radius = json_node.at("radius").get<double>();
            if(radius < 0) {
                throw std::invalid_argument("radius must be greater than 0");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'radius' key. Using " + std::to_string(radius));
        }

        try {
            color = json_node.at("color").get<std::vector<int>>();
            if(color.size() != 3) {
                throw std::invalid_argument("color must be specified in the form: [R, G, B]");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'color' key. Using " + vector_to_string(color));
        }

        m_center = Point_3(center);
        m_radius = radius;
        m_color = Color(color[0], color[1], color[2]);
    }

    Color Sphere::getColorAt(const Point_3& point) const
    {
        return m_color;
    }

    Vector_3 Sphere::getNormalAt(const linear_algebra_core::Point_3& point) const
    {
        return (m_center - point).normalize();
    }
}
