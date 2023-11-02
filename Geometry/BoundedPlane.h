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
#include "LinearAlgebraTypeTraits.h"
#include "LinearAlgebraJsonParser.h"

namespace geometry
{
    using namespace linear_algebra_core;
    using namespace color_core;

    template<IsFloatingPoint value_type>
    class BoundedPlane : public Geometry<value_type>
    {
    private:
        using Ray_3 = Ray<3, value_type>;
        using Point_3 = Point_X<3, value_type>;
        using Vector_3 = Vector_X<3, value_type>;

        Point_3  m_center{};
        Vector_3 m_normal{};
        value_type   m_rotationAngle{};
        value_type   m_width{};
        value_type   m_height{};
        Color    m_color{};

    public:
        BoundedPlane() = default;
        ~BoundedPlane() override = default;
        BoundedPlane(const BoundedPlane& other) = default;
        BoundedPlane(BoundedPlane&& other) noexcept = default;
        BoundedPlane& operator=(const BoundedPlane& other) = default;
        BoundedPlane& operator=(BoundedPlane&& other) noexcept = default;

        BoundedPlane(const Point_3& plane_center, const Vector_3& plane_normal, const Color& color,
                     value_type width, value_type height, value_type rotationAngle)
                     : m_center(plane_center), m_normal(plane_normal.getUnitVector()), m_color(color),
                       m_width(width), m_height(height), m_rotationAngle(rotationAngle) { }

        /*!
         * Determines if the given /p ray intersects this plane.
         * @param ray The ray to check for intersection
         * @return true if /p ray intersects this plane. False otherwise
         */
        [[nodiscard]] bool intersects(const Ray_3& ray) const override
        {
            std::optional<Point_3> intersection_point = getIntersectionPoint(ray);
            return intersection_point.has_value();
        }

        /*!
         * Determines if the given /p ray intersects this plane and returns the intersection point.
         * @param ray The ray to check for intersection
         * @return a std::optional containing the intersection point, if it exists. containing nothing, otherwise.
         * @details uses the solution described here: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
         */
        [[nodiscard]] std::optional<Point_3> getIntersectionPoint(const Ray_3& ray) const override
        {
            value_type denominator = m_normal * ray.getDirection();
            static const value_type epsilon = 1e-6;
            if(denominator <= epsilon) {
                return std::nullopt;
            }
            Vector_3 ray_origin_to_plane_center = m_center - ray.getOrigin();
            value_type t = (ray_origin_to_plane_center * m_normal) / denominator;
            Point_3 intersection_point = ray * t;

            //TODO: figure out if the intersection point is within the plane bounds

            return { intersection_point };
        }

        /*!
         * Retrieves the color at the given \p point on the plane. \p point is assumed to be on the plane.
         * @param point The point to get the color at
         * @return The Color at the given \p point
         */
        [[nodiscard]] Color getColorAt(const Point_3& point) const override
        {
            return m_color;
        }

        /*!
         * Retrieves the normal at the given \p point on the plane. \p point is assumed to be on the plane.
         * @param point The point to get the normal at
         * @return The normal at the given \p point
         */
        [[nodiscard]] Vector_3 getNormalAt(const Point_3& point) const override
        {
            return m_normal;
        }

        /*!
         * Construct a plane from the given \p json_node
         * @param json_node the json containing the parameters to construct the plane
         */
        void fromJson(const nlohmann::json& json_node) override
        {
            std::vector<value_type> center, normal;
            nlohmann::json color_json, center_json, normal_json;
            value_type width, height, rotationAngle;

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

            m_center = utility::PointFromJson<3, value_type>(center_json);
            m_normal = utility::VectorFromJson<3, value_type>(normal_json);
            m_color.fromJson(color_json);
            m_width = width;
            m_height = height;
            m_rotationAngle = rotationAngle;
        }

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
        [[nodiscard]] value_type getWidth() const { return m_width; }

        /*!
         * @return The plane height
         */
        [[nodiscard]] value_type getHeight() const { return m_height; }

        /*!
         * angle the plane is rotated about the normal
         * @return The plane rotation angle
         */
        [[nodiscard]] value_type getRotationAngle() const { return m_rotationAngle; }
    };
}