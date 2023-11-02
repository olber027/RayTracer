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
#include "LinearAlgebraTypeTraits.h"
#include "LinearAlgebraJsonParser.h"

namespace geometry
{
    using namespace linear_algebra_core;
    using namespace color_core;

    template<IsFloatingPoint value_type>
    class Sphere : public Geometry<value_type>
    {
    private:
        using Ray_3 = Ray<3, value_type>;
        using Point_3 = Point_X<3, value_type>;
        using Vector_3 = Vector_X<3, value_type>;

        Point_3 m_center;
        value_type  m_radius{};
        Color   m_color{};

    public:
        Sphere() = default;
        ~Sphere() override = default;
        Sphere(const Sphere& other) = default;
        Sphere(Sphere&& other) noexcept = default;
        Sphere& operator=(const Sphere& other) = default;
        Sphere& operator=(Sphere&& other) noexcept = default;

        Sphere(const Point_3& sphere_center, value_type sphere_radius, const Color& color) : m_center(sphere_center), m_radius(sphere_radius), m_color(color) { }

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
            Vector_3 center_to_ray_origin = m_center - ray.getOrigin();
            value_type t_projection_of_center_to_ray = center_to_ray_origin * ray.getDirection();
            if (t_projection_of_center_to_ray < 0) {
                return std::nullopt;
            }
            value_type projected_center_to_center_distance_squared = (center_to_ray_origin * center_to_ray_origin) - (t_projection_of_center_to_ray * t_projection_of_center_to_ray);
            value_type radius_squared = m_radius * m_radius;
            if (projected_center_to_center_distance_squared > radius_squared)
            {
                return std::nullopt;
            }
            value_type t_projected_center_to_sphere_surface = std::sqrt(radius_squared - projected_center_to_center_distance_squared);
            value_type t_first_intersection = t_projection_of_center_to_ray - t_projected_center_to_sphere_surface;
            value_type t_second_intersection = t_projection_of_center_to_ray + t_projected_center_to_sphere_surface;

            if (t_first_intersection > t_second_intersection) {
                std::swap(t_first_intersection, t_second_intersection);
            }

            if (t_first_intersection < 0)
            {
                return std::nullopt;  // intersection point is behind the ray
            }
            return {ray * t_first_intersection};
        }

        /*!
         * Retrieves the color at the given \p point on the sphere. \p point is assumed to be on the sphere.
         * @param point The point to get the color at
         * @return The Color at the given \p point
         */
        [[nodiscard]] Color getColorAt(const Point_3& point) const override
        {
            return m_color;
        }

        /*!
         * Retrieves the normal at the given \p point on the sphere. \p point is assumed to be on the sphere.
         * @param point The point to get the normal at
         * @return The normal at the given \p point
         */
        [[nodiscard]] Vector_3 getNormalAt(const Point_3& point) const override
        {
            return (m_center - point).normalize();
        }

        /*!
         * Construct a sphere from the given \p json_node
         * @param json_node the json containing the parameters to construct the sphere
         */
        void fromJson(const nlohmann::json& json_node) override
        {
            value_type radius;
            nlohmann::json color_json, center_json;

            try {
                center_json = json_node.at("center");
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'center' key.");
            }

            try {
                radius = json_node.at("radius").get<decltype(radius)>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'radius' key.");
            }
            if(radius < 0) {
                throw std::invalid_argument("radius must be greater than 0");
            }

            try {
                color_json = json_node.at("color");
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'color' key.");
            }


            m_center = utility::PointFromJson<3, value_type>(center_json);
            m_radius = radius;
            m_color.fromJson(color_json);
        }

        /*!
         * @return The sphere center
         */
        [[nodiscard]] Point_3 getCenter() const { return m_center; }

        /*!
         * @return The sphere radius
         */
        [[nodiscard]] value_type  getRadius() const { return m_radius; }

    };
}