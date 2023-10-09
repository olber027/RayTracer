//
// Created by Matt on 10/4/2023.
//

#include "Triangle.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"

namespace geometry {
    using namespace linear_algebra_core;
    using namespace color_core;

    bool Triangle::intersects(const Ray& ray) const
    {
        std::optional<Point_3> intersection_point = getIntersectionPoint(ray);
        return intersection_point.has_value();
    }

    std::optional<Point_3> Triangle::getIntersectionPoint(const Ray& ray) const
    {
        // https://stackoverflow.com/questions/42740765/intersection-between-line-and-triangle-in-3d
        static const double epsilon = 1e-6;
        auto edge_1 = m_corners[1] - m_corners[0];
        auto edge_2 = m_corners[2] - m_corners[0];
        auto determinant = -1 * (ray.getDirection() * m_normal);
        auto inverse_determinant = 1.0 / determinant;
        auto A_to_Ray_Origin = ray.getOrigin() - m_corners[0];
        auto DAO = A_to_Ray_Origin.cross(ray.getDirection());
        auto u = edge_2 * DAO * inverse_determinant;
        auto v = -1 * (edge_1 * DAO) * inverse_determinant;
        auto t = (A_to_Ray_Origin * m_normal) * inverse_determinant;
        if(determinant >= epsilon && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0)
        {
            return ray * t;
        }
        return std::nullopt;
    }

    void Triangle::fromJson(const nlohmann::json& json_node)
    {
        std::vector<std::vector<double>> corners;
        nlohmann::json color_json;

        try {
            corners = json_node.at("corners").get<decltype(corners)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'corners' key.");
        }
        if(corners.size() != 3)
        {
            throw std::invalid_argument("'corners' field must be specified in the form: [ [X, Y, Z], [X, Y, Z], [X, Y, Z] ]");
        }

        try {
            color_json = json_node.at("color");
        } catch(std::exception& e) {
            throw std::invalid_argument("Could not find the required 'color' key.");
        }

        m_corners = {Point_3{corners[0]}, Point_3{corners[1]}, Point_3{corners[2]}};
        m_normal = calculateNormal();
        m_color.fromJson(color_json);
    }
}
