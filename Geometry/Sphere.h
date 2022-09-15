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
        Color   m_color;

    public:
        Sphere() = default;
        ~Sphere() = default;
        Sphere(const Sphere& other) = default;
        Sphere(Sphere&& other) noexcept = default;
        Sphere& operator=(const Sphere& other) = default;
        Sphere& operator=(Sphere&& other) noexcept = default;

        Sphere(const Point_3& sphere_center, double sphere_radius, const Color& color) : m_center(sphere_center), m_radius(sphere_radius), m_color(color) { }

        [[nodiscard]] bool intersects(const Ray& ray) const override;
        [[nodiscard]] std::optional<Point_3> intersectsAt(const Ray& ray) const override;
        [[nodiscard]] Color getColorAt(const Point_3& point) const override;
        void fromJson(const nlohmann::json& json_node) override;

        [[nodiscard]] Point_3 getCenter() const { return m_center; }
        [[nodiscard]] double  getRadius() const { return m_radius; }

    };
}