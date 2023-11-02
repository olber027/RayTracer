//
// Created by Matt on 10/4/2023.
//
#pragma once

#include <optional>
#include <array>

#include "Geometry.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"
#include "LinearAlgebraTypeTraits.h"
#include "LinearAlgebraJsonParser.h"

namespace geometry
{
    using namespace linear_algebra_core;
    using namespace color_core;

    template<IsFloatingPoint value_type>
    class Triangle : public Geometry<value_type>
    {
    private:
        using Ray_3 = Ray<3, value_type>;
        using Point_3 = Point_X<3, value_type>;
        using Vector_3 = Vector_X<3, value_type>;

        std::array<Point_3, 3> m_corners;
        Vector_3 m_normal;
        Color   m_color{};

        [[nodiscard]] Vector_3 calculateNormal() const {
            return ((m_corners[1] - m_corners[0]).cross(m_corners[2] - m_corners[0])).normalize();
        }

    public:
        Triangle() = default;
        ~Triangle() override = default;
        Triangle(const Triangle& other) = default;
        Triangle(Triangle&& other) noexcept = default;
        Triangle& operator=(const Triangle& other) = default;
        Triangle& operator=(Triangle&& other) noexcept = default;

        Triangle(const std::array<Point_3, 3>& corners, const Color& color) :
            m_corners{corners},
            m_normal{calculateNormal()},
            m_color{color}
        { }

        Triangle(const Point_3& a, const Point_3& b, const Point_3& c, const Color& color) :
            m_corners{a, b, c},
            m_normal{calculateNormal()},
            m_color{color}
        { }

        /*!
         * Determines if the given /p ray intersects this sphere.
         * @param ray The ray to check for intersection
         * @return true if /p ray intersects this sphere. False otherwise
         */
        [[nodiscard]] bool intersects(const Ray_3& ray) const override
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
        [[nodiscard]] std::optional<Point_3> getIntersectionPoint(const Ray_3& ray) const override
        {
            // https://stackoverflow.com/questions/42740765/intersection-between-line-and-triangle-in-3d
            static const value_type epsilon = 1e-6;
            auto edge_1 = m_corners[1] - m_corners[0];
            auto edge_2 = m_corners[2] - m_corners[0];
            value_type determinant = -1 * (ray.getDirection() * m_normal);
            value_type inverse_determinant = 1.0 / determinant;
            auto A_to_Ray_Origin = ray.getOrigin() - m_corners[0];
            auto DAO = A_to_Ray_Origin.cross(ray.getDirection());
            value_type u = edge_2 * DAO * inverse_determinant;
            value_type v = -1 * (edge_1 * DAO) * inverse_determinant;
            value_type t = (A_to_Ray_Origin * m_normal) * inverse_determinant;
            if(determinant >= epsilon && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0)
            {
                return ray * t;
            }
            return std::nullopt;
        }

        /*!
         * Retrieves the color at the given \p point on the sphere. \p point is assumed to be on the sphere.
         * @param point The point to get the color at
         * @return The Color at the given \p point
         */
        [[nodiscard]] Color getColorAt(const Point_3& point) const override { return m_color; }

        /*!
         * Retrieves the normal at the given \p point on the sphere. \p point is assumed to be on the sphere.
         * @param point The point to get the normal at
         * @return The normal at the given \p point
         */
        [[nodiscard]] Vector_3 getNormalAt(const Point_3& point) const override { return m_normal; }

        /*!
         * Construct a sphere from the given \p json_node
         * @param json_node the json containing the parameters to construct the sphere
         */
        void fromJson(const nlohmann::json& json_node) override
        {
            std::vector<std::vector<value_type>> corners;
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

        /*!
         * @return The corners of the triangle
         */
        [[nodiscard]] const std::array<Point_3, 3>& getCorners() { return m_corners; }
        [[nodiscard]] std::array<Point_3, 3> getCorners() const { return m_corners; }
    };
}
