//
// Created by olber on 8/27/2022.
//
#pragma once

#include <optional>

#include "Geometry.h"
#include "Point_X.h"
#include "Vector_X.h"
#include "Color.h"
#include "json.h"

namespace geometry
{
    using namespace linear_algebra_core;
    using namespace color_core;

    class Sphere : public Geometry
    {
    private:
        Point_3 m_center;
        double  m_radius{};
        Color   m_color{};

    public:
        Sphere() = default;
        ~Sphere() override = default;
        Sphere(const Sphere& other) = default;
        Sphere(Sphere&& other) noexcept = default;
        Sphere& operator=(const Sphere& other) = default;
        Sphere& operator=(Sphere&& other) noexcept = default;

        Sphere(const Point_3& sphere_center, double sphere_radius, const Color& color) : m_center(sphere_center), m_radius(sphere_radius), m_color(color) { }

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
        [[nodiscard]] Color getColorAt(const Point_3& point) const override;

        /*!
         * Retrieves the normal at the given \p point on the sphere. \p point is assumed to be on the sphere.
         * @param point The point to get the normal at
         * @return The normal at the given \p point
         */
        [[nodiscard]] Vector_3 getNormalAt(const Point_3& point) const override;

        /*!
         * Construct a sphere from the given \p json_node
         * @param json_node the json containing the parameters to construct the sphere
         */
        void fromJson(const nlohmann::json& json_node) override;

        /*!
         * @return The sphere center
         */
        [[nodiscard]] Point_3 getCenter() const { return m_center; }

        /*!
         * @return The sphere radius
         */
        [[nodiscard]] double  getRadius() const { return m_radius; }

    };
}