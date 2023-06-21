//
// Created by olber on 10/6/2022.
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

    class BoundedPlane : public Geometry
    {
    private:
        Point_3  m_center{};
        Vector_3 m_normal{};
        double   m_rotationAngle{};
        double   m_width{};
        double   m_height{};
        Color    m_color{};

    public:
        BoundedPlane() = default;
        ~BoundedPlane() override = default;
        BoundedPlane(const BoundedPlane& other) = default;
        BoundedPlane(BoundedPlane&& other) noexcept = default;
        BoundedPlane& operator=(const BoundedPlane& other) = default;
        BoundedPlane& operator=(BoundedPlane&& other) noexcept = default;

        BoundedPlane(const Point_3& plane_center, const Vector_3& plane_normal, const Color& color,
                     double width, double height, double rotationAngle)
                     : m_center(plane_center), m_normal(plane_normal.getUnitVector()), m_color(color),
                       m_width(width), m_height(height), m_rotationAngle(rotationAngle) { }

        /*!
         * Determines if the given /p ray intersects this plane.
         * @param ray The ray to check for intersection
         * @return true if /p ray intersects this plane. False otherwise
         */
        [[nodiscard]] bool intersects(const Ray& ray) const override;

        /*!
         * Determines if the given /p ray intersects this plane and returns the intersection point.
         * @param ray The ray to check for intersection
         * @return a std::optional containing the intersection point, if it exists. containing nothing, otherwise.
         * @details uses the solution described here: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
         */
        [[nodiscard]] std::optional<Point_3> getIntersectionPoint(const Ray& ray) const override;

        /*!
         * Retrieves the color at the given \p point on the plane. \p point is assumed to be on the plane.
         * @param point The point to get the color at
         * @return The Color at the given \p point
         */
        [[nodiscard]] Color getColorAt(const Point_3& point) const override;

        /*!
         * Retrieves the normal at the given \p point on the plane. \p point is assumed to be on the plane.
         * @param point The point to get the normal at
         * @return The normal at the given \p point
         */
        [[nodiscard]] Vector_3 getNormalAt(const Point_3& point) const override;

        /*!
         * Construct a plane from the given \p json_node
         * @param json_node the json containing the parameters to construct the plane
         */
        void fromJson(const nlohmann::json& json_node) override;

        /*!
         * @return The plane center
         */
        [[nodiscard]] Point_3 getCenter() const { return m_center; }

        /*!
         * @return The plane normal
         */
        [[nodiscard]] Vector_3 getNormal() const { return m_normal; }

        /*!
         * @return The plane width
         */
        [[nodiscard]] double getWidth() const { return m_width; }

        /*!
         * @return The plane height
         */
        [[nodiscard]] double getHeight() const { return m_height; }

        /*!
         * angle the plane is rotated about the normal
         * @return The plane rotation angle
         */
        [[nodiscard]] double getRotationAngle() const { return m_rotationAngle; }
    };
}