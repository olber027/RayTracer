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

namespace geometry
{
    using namespace linear_algebra_core;
    using namespace color_core;

    class Triangle : public Geometry
    {
    private:
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
        [[nodiscard]] bool intersects(const Ray& ray) const override;

        /*!
         * Determines if the given /p ray intersects this sphere and returns the first intersection point.
         * @param ray The ray to check for intersection
         * @return a std::optional containing the first intersection point, if it exists. containing nothing, otherwise.
         * @details uses the geometric solution described here: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
         */
        [[nodiscard]] std::optional<Point_3> getIntersectionPoint(const Ray& ray) const override;

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
        void fromJson(const nlohmann::json& json_node) override;

        /*!
         * @return The corners of the triangle
         */
        [[nodiscard]] const std::array<Point_3, 3>& getCorners() { return m_corners; }
        [[nodiscard]] std::array<Point_3, 3> getCorners() const { return m_corners; }
    };
}
