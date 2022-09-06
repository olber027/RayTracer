//
// Created by olber on 8/27/2022.
//
#pragma once

#include <optional>

#include "Geometry.h"
#include "Point_X.h"
#include "Vector_X.h"

namespace geometry
{
    using namespace linear_algebra_core;

    class Sphere : public Geometry
    {
    private:
        Point_3 m_center;
        double  m_radius;

    public:
        Sphere(Point_3 sphere_center, double sphere_radius) : m_center(sphere_center), m_radius(sphere_radius) { }
        bool intersects(const Ray& ray) const override;

        std::optional<Point_3> intersectsAt(const Ray& ray) const override;
    };
}