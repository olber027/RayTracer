//
// Created by olber on 10/3/2022.
//

#include "Plane.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"
#include "LinearAlgebraJsonParser.h"

namespace geometry {
    using namespace linear_algebra_core;
    using namespace color_core;

    bool Plane::intersects(const Ray& ray) const
    {
        std::optional<Point_3> intersection_point = getIntersectionPoint(ray);
        return intersection_point.has_value();
    }

    std::optional<Point_3> Plane::getIntersectionPoint(const Ray& ray) const
    {
        double denominator = m_normal * ray.getDirection();
        static const double epsilon = 1e-6;
        if(denominator <= epsilon) {
            return std::nullopt;
        }
        Vector_3 ray_origin_to_plane_center = m_center - ray.getOrigin();
        double t = (ray_origin_to_plane_center * m_normal) / denominator;
        return { ray * t };
    }

    void Plane::fromJson(const nlohmann::json& json_node)
    {
        nlohmann::json color_json, center_json, normal_json;

        try {
            center_json = json_node.at("center");
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'center' key.");
        }

        try {
            normal_json = json_node.at("normal");
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'normal' key.");
        }

        try {
            color_json = json_node.at("color");
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'color' key.");
        }

        m_center = utility::PointFromJson<3>(center_json);
        m_normal = utility::VectorFromJson<3>(normal_json);
        m_color.fromJson(color_json);
    }

    Color Plane::getColorAt(const Point_3& point) const
    {
        return m_color;
    }

    Vector_3 Plane::getNormalAt(const Point_3& point) const
    {
        return m_normal;
    }
}