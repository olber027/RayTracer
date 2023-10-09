//
// Created by olber on 10/6/2022.
//

#include "BoundedPlane.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"
#include "LinearAlgebraJsonParser.h"

namespace geometry {
    using namespace linear_algebra_core;
    using namespace color_core;

    bool BoundedPlane::intersects(const Ray& ray) const
    {
        std::optional<Point_3> intersection_point = getIntersectionPoint(ray);
        return intersection_point.has_value();
    }

    std::optional<Point_3> BoundedPlane::getIntersectionPoint(const Ray& ray) const
    {
        double denominator = m_normal * ray.getDirection();
        static const double epsilon = 1e-6;
        if(denominator <= epsilon) {
            return std::nullopt;
        }
        Vector_3 ray_origin_to_plane_center = m_center - ray.getOrigin();
        double t = (ray_origin_to_plane_center * m_normal) / denominator;
        Point_3 intersection_point = ray * t;

        //TODO: figure out if the intersection point is within the plane bounds

        return { intersection_point };
    }

    void BoundedPlane::fromJson(const nlohmann::json& json_node)
    {
        std::vector<double> center, normal;
        nlohmann::json color_json, center_json, normal_json;
        double width, height, rotationAngle;

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

        try {
            width = json_node.at("width").get<decltype(width)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'width' key.");
        }
        if(width < 0) {
            throw std::invalid_argument("width cannot have a negative value");
        }

        try {
            height = json_node.at("height").get<decltype(height)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'height' key.");
        }
        if(height < 0) {
            throw std::invalid_argument("height cannot have a negative value");
        }

        try {
            rotationAngle = json_node.at("rotation_angle").get<decltype(rotationAngle)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'rotation_angle' key.");
        }

        m_center = utility::PointFromJson<3>(center_json);
        m_normal = utility::VectorFromJson<3>(normal_json);
        m_color.fromJson(color_json);
        m_width = width;
        m_height = height;
        m_rotationAngle = rotationAngle;
    }

    Color BoundedPlane::getColorAt(const Point_3& point) const
    {
        return m_color;
    }

    Vector_3 BoundedPlane::getNormalAt(const Point_3& point) const
    {
        return m_normal;
    }
}