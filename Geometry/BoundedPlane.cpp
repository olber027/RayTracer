//
// Created by olber on 10/6/2022.
//

#include "BoundedPlane.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"

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
        std::vector<double> center {0, 0, 0};
        std::vector<double> normal {0, 0, 0};
        std::vector<int> color {0, 0, 0};
        double width = 0;
        double height = 0;
        double rotationAngle = 0;
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
            normal = json_node.at("normal").get<std::vector<double>>();
            if(center.size() != 3)
            {
                throw std::invalid_argument("'normal' field must be specified in the form: [X, Y, Z]");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'normal' key. Using " + vector_to_string(normal));
        }

        try {
            color = json_node.at("color").get<std::vector<int>>();
            if(color.size() != 3) {
                throw std::invalid_argument("color must be specified in the form: [R, G, B]");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'color' key. Using " + vector_to_string(color));
        }

        try {
            width = json_node.at("width").get<double>();
            if(width < 0) {
                throw std::invalid_argument("width cannot have a negative value");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'width' key. Using " + std::to_string(width));
        }

        try {
            height = json_node.at("height").get<double>();
            if(height < 0) {
                throw std::invalid_argument("height cannot have a negative value");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'height' key. Using " + std::to_string(height));
        }

        try {
            rotationAngle = json_node.at("rotation_angle").get<double>();
            if(rotationAngle < 0) {
                throw std::invalid_argument("rotation_angle cannot have a negative value");
            }
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'rotation_angle' key. Using " + std::to_string(rotationAngle));
        }

        m_center = Point_3(center);
        m_normal = Vector_3(normal);
        m_color = Color(color[0], color[1], color[2]);
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